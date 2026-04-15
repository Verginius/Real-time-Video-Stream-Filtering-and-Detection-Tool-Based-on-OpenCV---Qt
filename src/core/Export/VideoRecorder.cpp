#include "VideoRecorder.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

VideoRecorder::VideoRecorder(RecordConfig cfg)
    : m_cfg(std::move(cfg)), m_recording(false), m_frameCount(0), m_droppedFrames(0), m_stopIo(false)
{
}

VideoRecorder::~VideoRecorder()
{
    stop();
}

std::string VideoRecorder::generateFilename() const
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << m_cfg.prefix << "_";
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    ss << ".mp4";
    return ss.str();
}

bool VideoRecorder::start()
{
    if (m_recording) return true;

    if (!std::filesystem::exists(m_cfg.outputDir)) {
        try {
            std::filesystem::create_directories(m_cfg.outputDir);
        } catch (const std::exception& e) {
            std::cerr << "Failed to create output directory: " << e.what() << std::endl;
            return false;
        }
    }

    std::string filename = generateFilename();
    m_currentPath = m_cfg.outputDir / filename;

    m_recording = true;
    m_frameCount = 0;
    m_droppedFrames = 0;
    m_stopIo = false;

    // Start IO thread
    m_ioThread = std::thread(&VideoRecorder::ioThreadFunc, this);

    return true;
}

void VideoRecorder::writeFrame(const cv::Mat& frame)
{
    if (!m_recording || frame.empty()) return;

    std::unique_lock<std::mutex> lock(m_queueMutex);
    
    if (m_queue.size() >= m_cfg.maxQueueSize) {
        m_queue.pop_front();
        m_droppedFrames++;
    }
    
    m_queue.push_back(frame.clone());
    m_queueCv.notify_one();
}

std::filesystem::path VideoRecorder::stop()
{
    if (!m_recording) return {};

    m_recording = false;
    
    // Signal IO thread to stop
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_stopIo = true;
    }
    m_queueCv.notify_all();

    if (m_ioThread.joinable()) {
        m_ioThread.join();
    }

    if (m_writer.isOpened()) {
        m_writer.release();
    }
    
    // Clear queue
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_queue.clear();
    }

    return m_currentPath;
}

bool VideoRecorder::isRecording() const
{
    return m_recording;
}

std::size_t VideoRecorder::frameCount() const
{
    return m_frameCount;
}

double VideoRecorder::durationSec() const
{
    if (m_cfg.fps <= 0) return 0.0;
    return static_cast<double>(m_frameCount) / m_cfg.fps;
}

std::size_t VideoRecorder::droppedFrames() const
{
    return m_droppedFrames;
}

void VideoRecorder::ioThreadFunc()
{
    bool writerInitialized = false;

    while (true) {
        cv::Mat frame;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_queueCv.wait(lock, [this] { return !m_queue.empty() || m_stopIo; });

            if (m_queue.empty() && m_stopIo) {
                break;
            }

            if (!m_queue.empty()) {
                frame = std::move(m_queue.front());
                m_queue.pop_front();
            }
        }

        if (!frame.empty()) {
            if (!writerInitialized) {
                if (!m_writer.open(m_currentPath.string(), m_cfg.fourcc, m_cfg.fps, frame.size(), frame.channels() == 3)) {
                    std::cerr << "Failed to open VideoWriter for path: " << m_currentPath << std::endl;
                    m_recording = false;
                    break;
                }
                writerInitialized = true;
            }

            if (m_writer.isOpened()) {
                m_writer.write(frame);
                m_frameCount++;
            }
        }
    }
    
    // Process remaining frames in queue
    while (true) {
        cv::Mat frame;
        {
             std::unique_lock<std::mutex> lock(m_queueMutex);
             if(m_queue.empty()) break;
             frame = std::move(m_queue.front());
             m_queue.pop_front();
        }
        
        if (!frame.empty() && m_writer.isOpened()) {
             m_writer.write(frame);
             m_frameCount++;
        }
    }
}