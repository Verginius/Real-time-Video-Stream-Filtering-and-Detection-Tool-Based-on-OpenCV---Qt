#include "FramePipeline.h"
#include <thread>
#include <future>
#include <atomic>

AsyncPipeline::AsyncPipeline()
    : m_running(false)
    , m_stopRequested(false)
{
}

AsyncPipeline::~AsyncPipeline() {
    stop();
}

void AsyncPipeline::setFilterProcessor(FilterProcessor processor) {
    m_filterProcessor = std::move(processor);
}

void AsyncPipeline::setDetector(DetectorFunc detector) {
    m_detector = std::move(detector);
}

void AsyncPipeline::setCallback(FrameCallback callback) {
    m_callback = std::move(callback);
}

void AsyncPipeline::start() {
    if (m_running.exchange(true)) return;
    m_stopRequested = false;
    m_processThread = std::thread(&AsyncPipeline::processLoop, this);
}

void AsyncPipeline::stop() {
    if (!m_running.exchange(false)) return;
    m_stopRequested = true;
    m_queueCV.notify_all();
    if (m_processThread.joinable()) {
        m_processThread.join();
    }
}

bool AsyncPipeline::isRunning() const {
    return m_running.load();
}

void AsyncPipeline::enqueueFrame(cv::Mat frame) {
    if (!m_running.load()) return;
    
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_frameQueue.push({std::move(frame), nullptr});
    }
    m_queueCV.notify_one();
}

void AsyncPipeline::processLoop() {
    while (!m_stopRequested.load()) {
        AsyncFrame asyncFrame;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_queueCV.wait(lock, [this] { 
                return !m_frameQueue.empty() || m_stopRequested.load();
            });
            
            if (m_stopRequested.load() && m_frameQueue.empty()) break;
            if (m_frameQueue.empty()) continue;
            
            asyncFrame = std::move(m_frameQueue.front());
            m_frameQueue.pop();
        }

        cv::Mat original = asyncFrame.frame.clone();
        
        cv::Mat processed;
        if (m_filterProcessor) {
            processed = m_filterProcessor(asyncFrame.frame);
        } else {
            processed = asyncFrame.frame.clone();
        }
        
        DetectionList detections;
        if (m_detector) {
            detections = m_detector(processed);
        }

        if (m_callback) {
            m_callback(std::move(original), std::move(processed), std::move(detections));
        }
    }
}

cv::Mat AsyncPipeline::processFiltersAsync(const cv::Mat& src) {
    if (m_filterProcessor) {
        return m_filterProcessor(src);
    }
    return src.clone();
}

DetectionList AsyncPipeline::processDetectionAsync(const cv::Mat& src) {
    if (m_detector) {
        return m_detector(src);
    }
    return {};
}
