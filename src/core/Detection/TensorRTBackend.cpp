#ifdef HAVE_TENSORRT
#include "TensorRTBackend.h"
#include "LabelMap.h"
#include <NvInferRuntime.h>
#include <nvonnxparser.h>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <iostream>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

TensorRTBackend::TensorRTBackend() {
    m_inputBuffer.resize(3 * m_inputSize.width * m_inputSize.height);
    m_outputBuffer.resize(84 * 8400);

    cudaStream_t stream;
    cudaStreamCreate(&stream);
    m_cudaStream = stream;
}

TensorRTBackend::~TensorRTBackend() {
    if (m_deviceBuffer) {
        cudaFree(m_deviceBuffer);
        m_deviceBuffer = nullptr;
    }
    if (m_cudaStream) {
        cudaStreamDestroy(reinterpret_cast<cudaStream_t>(m_cudaStream));
        m_cudaStream = nullptr;
    }
}

bool TensorRTBackend::loadModel(const std::string& modelPath, const std::string&) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_modelPath = modelPath;
    m_enginePath = modelPath + ".engine";

    if (!loadEngine(m_enginePath)) {
        if (!buildEngine(modelPath, m_enginePath)) {
            return false;
        }
    }
    m_loaded = true;
    return true;
}

bool TensorRTBackend::buildEngine(const std::string& onnxPath, const std::string& enginePath) {
    std::cout << "Building TensorRT engine from ONNX: " << onnxPath << std::endl;
    
    nvinfer1::IBuilder* builder = nvinfer1::createInferBuilder(m_logger);
    if (!builder) {
        std::cerr << "Failed to create TensorRT builder" << std::endl;
        return false;
    }
    
    nvinfer1::INetworkDefinition* network = builder->createNetworkV2(1);
    
    std::ifstream file(onnxPath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open ONNX file: " << onnxPath << std::endl;
        return false;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    file.close();
    
    nvonnxparser::IParser* parser = nvonnxparser::createParser(*network, m_logger);
    if (!parser) {
        std::cerr << "Failed to create ONNX parser" << std::endl;
        return false;
    }
    
    parser->parse(buffer.data(), size);
    
    auto config = builder->createBuilderConfig();
    config->setMemoryPoolLimit(nvinfer1::MemoryPoolType::kWORKSPACE, 1 << 30);
    
    nvinfer1::IHostMemory* engineData = builder->buildSerializedNetwork(*network, *config);
    if (!engineData) {
        std::cerr << "Failed to build TensorRT engine" << std::endl;
        return false;
    }

    std::ofstream engineFile(enginePath, std::ios::binary);
    engineFile.write(static_cast<char*>(engineData->data()), engineData->size());
    engineFile.close();

    m_runtime = std::unique_ptr<nvinfer1::IRuntime>(nvinfer1::createInferRuntime(m_logger));
    m_engine = std::unique_ptr<nvinfer1::ICudaEngine>(
        m_runtime->deserializeCudaEngine(engineData->data(), engineData->size()));
    m_context = std::unique_ptr<nvinfer1::IExecutionContext>(
        m_engine->createExecutionContext());
    
    return true;
}

bool TensorRTBackend::loadEngine(const std::string& enginePath) {
    std::ifstream file(enginePath, std::ios::binary);
    if (!file) {
        std::cout << "TensorRT engine not found, will build from ONNX" << std::endl;
        return false;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    file.close();

    m_runtime = std::unique_ptr<nvinfer1::IRuntime>(
        nvinfer1::createInferRuntime(m_logger));
    if (!m_runtime) {
        std::cerr << "Failed to create TensorRT runtime" << std::endl;
        return false;
    }
    
    m_engine = std::unique_ptr<nvinfer1::ICudaEngine>(
        m_runtime->deserializeCudaEngine(buffer.data(), size));
    if (!m_engine) {
        std::cerr << "Failed to deserialize TensorRT engine" << std::endl;
        return false;
    }

    m_context = std::unique_ptr<nvinfer1::IExecutionContext>(
        m_engine->createExecutionContext());
    if (!m_context) {
        std::cerr << "Failed to create TensorRT execution context" << std::endl;
        return false;
    }
    
    return true;
}

DetectionList TensorRTBackend::detect(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_loaded || frame.empty()) return {};

    double t0 = (double)cv::getTickCount();

    cv::Mat blob = preprocess(frame);
    
    std::vector<float> inputData = m_inputBuffer;
    std::vector<float> outputData = m_outputBuffer;

    const size_t inputSize = inputData.size() * sizeof(float);
    const size_t outputSize = outputData.size() * sizeof(float);

    if (!m_deviceBuffer) {
        cudaMalloc(&m_deviceBuffer, inputSize + outputSize);
    }

    void* deviceInput = m_deviceBuffer;
    void* deviceOutput = static_cast<char*>(m_deviceBuffer) + inputSize;

    cudaMemcpy(deviceInput, inputData.data(), inputSize, cudaMemcpyHostToDevice);

    m_context->executeV2(&deviceOutput);

    cudaMemcpy(outputData.data(), deviceOutput, outputSize, cudaMemcpyDeviceToHost);

    DetectionList results = postprocess(outputData.data(), outputData.size(), frame.size());

    m_lastInfMs = ((double)cv::getTickCount() - t0) * 1000.0 / cv::getTickFrequency();
    return results;
}

cv::Mat TensorRTBackend::preprocess(const cv::Mat& frame) {
    cv::Mat letterboxed;
    float scale = std::min(m_inputSize.width / float(frame.cols),
                           m_inputSize.height / float(frame.rows));
    cv::resize(frame, letterboxed,
               cv::Size(int(frame.cols * scale), int(frame.rows * scale)),
               0, 0, cv::INTER_LINEAR);
    cv::Mat padded(m_inputSize, CV_8UC3, cv::Scalar(114, 114, 114));
    letterboxed.copyTo(padded(cv::Rect(0, 0, letterboxed.cols, letterboxed.rows)));

    int idx = 0;
    for (int c = 0; c < 3; c++) {
        for (int h = 0; h < m_inputSize.height; h++) {
            for (int w = 0; w < m_inputSize.width; w++) {
                m_inputBuffer[idx++] = padded.at<cv::Vec3b>(h, w)[c] / 255.0f;
            }
        }
    }

    return padded;
}

DetectionList TensorRTBackend::postprocess(const float* output, int outputSize, const cv::Size& origSize) {
    DetectionList detections;
    const int numAnchors = 8400;
    const int numClasses = 80;
    const int numParams = 4 + numClasses;

    float scale = std::min(m_inputSize.width / float(origSize.width),
                           m_inputSize.height / float(origSize.height));
    float x_offset = (m_inputSize.width - origSize.width * scale) / 2.0f;
    float y_offset = (m_inputSize.height - origSize.height * scale) / 2.0f;

    std::vector<cv::Rect> boxes;
    std::vector<float> scores;
    std::vector<int> classIds;

    for (int i = 0; i < numAnchors; i++) {
        float cx = output[i * numParams];
        float cy = output[i * numParams + 1];
        float w = output[i * numParams + 2];
        float h = output[i * numParams + 3];

        float maxScore = 0.0f;
        int bestClass = 0;
        for (int c = 0; c < numClasses; c++) {
            float score = output[i * numParams + 4 + c];
            if (score > maxScore) {
                maxScore = score;
                bestClass = c;
            }
        }

        if (maxScore > m_confThresh) {
            float orig_cx = (cx - x_offset) / scale;
            float orig_cy = (cy - y_offset) / scale;
            float orig_w = w / scale;
            float orig_h = h / scale;

            boxes.push_back(cv::Rect(orig_cx - orig_w / 2, orig_cy - orig_h / 2, orig_w, orig_h));
            scores.push_back(maxScore);
            classIds.push_back(bestClass);
        }
    }

    std::vector<int> indices;
    std::vector<float> boxScores(boxes.size());
    for (size_t i = 0; i < boxes.size(); i++) boxScores[i] = scores[i];

    std::vector<int> keep;
    for (size_t i = 0; i < boxScores.size(); i++) {
        bool keep_box = true;
        for (size_t j = 0; j < keep.size(); j++) {
            float iou = 0.0f;
            cv::Rect r1 = boxes[i];
            cv::Rect r2 = boxes[keep[j]];
            cv::Rect inter = r1 & r2;
            if (inter.area() > 0) {
                float union_area = r1.area() + r2.area() - inter.area();
                iou = float(inter.area()) / union_area;
            }
            if (iou > m_nmsThresh) {
                keep_box = false;
                break;
            }
        }
        if (keep_box) {
            keep.push_back(i);
            if (keep.size() >= 100) break;
        }
    }

    LabelMap labels;
    labels.loadCOCO80();

    for (int idx : keep) {
        Detection det;
        det.bbox = boxes[idx];
        det.classId = classIds[idx];
        det.confidence = scores[idx];
        det.label = labels.nameOf(det.classId);
        detections.push_back(det);
    }

    return detections;
}

bool TensorRTBackend::isLoaded() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_loaded;
}

void TensorRTBackend::setConfThreshold(float t) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_confThresh = t;
}

float TensorRTBackend::confThreshold() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_confThresh;
}

void TensorRTBackend::setNmsThreshold(float t) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_nmsThresh = t;
}

float TensorRTBackend::nmsThreshold() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_nmsThresh;
}

double TensorRTBackend::lastInferenceMsec() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_lastInfMs;
}

#endif
