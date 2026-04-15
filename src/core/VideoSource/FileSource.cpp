#include "FileSource.h"

FileSource::FileSource(std::filesystem::path filePath)
    : m_path(std::move(filePath))
{
}

bool FileSource::open() {
    if (m_cap.isOpened()) {
        m_cap.release();
    }
    
    // Convert path to string (UTF-8) for OpenCV
    bool success = m_cap.open(m_path.string());

    if (success) {
        m_w = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
        m_h = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        m_fps = m_cap.get(cv::CAP_PROP_FPS);
        
        // Calculate duration based on frame count and fps
        double frameCount = m_cap.get(cv::CAP_PROP_FRAME_COUNT);
        if (m_fps > 0) {
            m_duration = (frameCount / m_fps) * 1000.0;
        }
        
        m_paused = false;
        m_lastFrame.release();
    }
    return success;
}

bool FileSource::read(cv::Mat& frame) {
    if (!m_cap.isOpened()) {
        return false;
    }
    
    if (m_paused) {
        if (!m_lastFrame.empty()) {
            m_lastFrame.copyTo(frame);
            return true;
        }
        return false; // Paused but no frame buffered yet
    }

    bool success = m_cap.read(frame);
    if (success) {
        frame.copyTo(m_lastFrame);
    } else {
        // EOF reached
    }
    return success;
}

void FileSource::close() {
    if (m_cap.isOpened()) {
        m_cap.release();
    }
    m_lastFrame.release();
}

bool FileSource::isOpened() const {
    return m_cap.isOpened();
}

int FileSource::width() const {
    return m_w;
}

int FileSource::height() const {
    return m_h;
}

double FileSource::fps() const {
    return m_fps > 0 ? m_fps : 30.0; // Default to 30 if unknown
}

std::string FileSource::description() const {
    return "File: " + m_path.filename().string();
}

void FileSource::pause() {
    m_paused = true;
}

void FileSource::resume() {
    m_paused = false;
}

bool FileSource::seek(double posMsec) {
    if (!m_cap.isOpened()) return false;
    
    bool success = m_cap.set(cv::CAP_PROP_POS_MSEC, posMsec);
    if (success) {
        // Clear the buffered frame so the next read grabs the new position
        m_lastFrame.release();
    }
    return success;
}

double FileSource::posMsec() const {
    if (!m_cap.isOpened()) return 0.0;
    return m_cap.get(cv::CAP_PROP_POS_MSEC);
}

double FileSource::durationMsec() const {
    return m_duration;
}

bool FileSource::stepForward() {
    if (!m_cap.isOpened() || !m_paused) return false;
    
    cv::Mat frame;
    if (m_cap.read(frame)) {
        frame.copyTo(m_lastFrame);
        return true;
    }
    return false;
}
