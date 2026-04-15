#include "YOLODetector.h"
#include <opencv2/imgproc.hpp>
#include <iostream>

YOLODetector::YOLODetector(YOLOConfig cfg) : m_cfg(cfg), m_inputSize(cfg.inputWidth, cfg.inputHeight) {
    m_labels.loadCOCO80();
}

bool YOLODetector::loadModel(const std::string& modelPath, const std::string& labelsPath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    try {
        m_net = cv::dnn::readNetFromONNX(modelPath);
        m_net.setPreferableBackend(m_cfg.backendId);
        m_net.setPreferableTarget(m_cfg.targetId);
        if (!labelsPath.empty()) {
            m_labels.loadFromFile(labelsPath);
        } else {
            m_labels.loadCOCO80();
        }
        m_loaded = !m_net.empty();
        return m_loaded;
    } catch (const cv::Exception& e) {
        std::cerr << "Exception loading model: " << e.what() << std::endl;
        m_loaded = false;
        return false;
    }
}

DetectionList YOLODetector::detect(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded || frame.empty()) return {};

    double t0 = (double)cv::getTickCount();

    cv::Mat blob = preprocess(frame);
    m_net.setInput(blob);

    std::vector<cv::Mat> outputs;
    m_net.forward(outputs, m_net.getUnconnectedOutLayersNames());

    DetectionList results = postprocess(outputs, frame.size());

    m_lastInfMs = ((double)cv::getTickCount() - t0) * 1000.0 / cv::getTickFrequency();
    return results;
}

bool YOLODetector::isLoaded() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_loaded;
}

void YOLODetector::setConfThreshold(float t) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cfg.confThresh = t;
}

float YOLODetector::confThreshold() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_cfg.confThresh;
}

void YOLODetector::setNmsThreshold(float t) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cfg.nmsThresh = t;
}

float YOLODetector::nmsThreshold() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_cfg.nmsThresh;
}

void YOLODetector::setBackend(int backendId, int targetId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cfg.backendId = backendId;
    m_cfg.targetId = targetId;
}

double YOLODetector::lastInferenceMsec() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_lastInfMs;
}

cv::Mat YOLODetector::preprocess(const cv::Mat& frame) {
    cv::Mat letterboxed;
    float scale = std::min(m_inputSize.width  / float(frame.cols),
                           m_inputSize.height / float(frame.rows));
    cv::resize(frame, letterboxed,
               cv::Size(int(frame.cols*scale), int(frame.rows*scale)),
               0, 0, cv::INTER_LINEAR);
    cv::Mat padded(m_inputSize, CV_8UC3, cv::Scalar(114,114,114));
    letterboxed.copyTo(padded(cv::Rect(0, 0, letterboxed.cols, letterboxed.rows)));
    
    cv::Mat blob;
    cv::dnn::blobFromImage(padded, blob, 1.0/255.0, m_inputSize, cv::Scalar(), true, false, CV_32F);
    return blob;
}

DetectionList YOLODetector::postprocess(const std::vector<cv::Mat>& outputs, const cv::Size& origSize) {
    DetectionList detections;
    if (outputs.empty()) return detections;

    // YOLOv8 output is [1, 84, 8400]
    cv::Mat output = outputs[0];
    if (output.dims == 3) {
        // [1, 84, 8400] -> [84, 8400]
        cv::Mat reshaped(output.size[1], output.size[2], CV_32F, output.ptr<float>());
        // Transpose to [8400, 84]
        cv::Mat transposed;
        cv::transpose(reshaped, transposed);

        std::vector<cv::Rect> boxes;
        std::vector<float> scores;
        std::vector<int> classIds;

        float scale = std::min(m_inputSize.width / float(origSize.width),
                               m_inputSize.height / float(origSize.height));
        float x_offset = (m_inputSize.width - origSize.width * scale) / 2.0f;
        float y_offset = (m_inputSize.height - origSize.height * scale) / 2.0f;

        // Iterate through rows (8400 anchors)
        for (int i = 0; i < transposed.rows; ++i) {
            float* row = transposed.ptr<float>(i);
            float cx = row[0];
            float cy = row[1];
            float w = row[2];
            float h = row[3];

            // Find best class
            cv::Mat classScores(1, transposed.cols - 4, CV_32F, row + 4);
            cv::Point classIdPoint;
            double maxClassScore;
            cv::minMaxLoc(classScores, nullptr, &maxClassScore, nullptr, &classIdPoint);

            if (maxClassScore > m_cfg.confThresh) {
                // Map back to original image coordinates
                float orig_cx = (cx - x_offset) / scale;
                float orig_cy = (cy - y_offset) / scale;
                float orig_w = w / scale;
                float orig_h = h / scale;

                float left = orig_cx - orig_w / 2.0f;
                float top = orig_cy - orig_h / 2.0f;

                boxes.push_back(cv::Rect(left, top, orig_w, orig_h));
                scores.push_back(maxClassScore);
                classIds.push_back(classIdPoint.x);
            }
        }

        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, scores, m_cfg.confThresh, m_cfg.nmsThresh, indices);

        for (int idx : indices) {
            Detection det;
            det.bbox = boxes[idx];
            det.classId = classIds[idx];
            det.confidence = scores[idx];
            det.label = m_labels.nameOf(det.classId);
            detections.push_back(det);
        }
    }
    return detections;
}
