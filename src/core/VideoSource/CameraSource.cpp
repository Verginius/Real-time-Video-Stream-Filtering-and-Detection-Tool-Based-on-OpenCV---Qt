#include "CameraSource.h"
#include <opencv2/videoio/registry.hpp>
#include <iostream>

std::vector<CameraInfo> CameraSource::enumerateDevices(int maxProbe) {
    std::vector<CameraInfo> cameras;
    // In a real Windows app with DirectShow, we might use cv::videoio_registry::getCameraBackends() 
    // or OS-specific APIs (e.g., IMFMediaSource) to get friendly names. 
    // For cross-platform simplicity with OpenCV, we probe indices.
    for (int i = 0; i < maxProbe; ++i) {
        cv::VideoCapture cap;
#ifdef _WIN32
        bool opened = cap.open(i, cv::CAP_DSHOW);
#else
        bool opened = cap.open(i, cv::CAP_V4L2);
#endif
        if (opened) {
            CameraInfo info;
            info.index = i;
            info.name = "Camera " + std::to_string(i);
            cameras.push_back(info);
            cap.release();
        }
    }
    return cameras;
}

CameraSource::CameraSource(int deviceIndex, int preferWidth, int preferHeight, int preferFps)
    : m_deviceIndex(deviceIndex), m_preferW(preferWidth), m_preferH(preferHeight), m_preferFps(preferFps)
{
}

bool CameraSource::open() {
    if (m_cap.isOpened()) {
        m_cap.release();
    }
    
#ifdef _WIN32
    bool success = m_cap.open(m_deviceIndex, cv::CAP_DSHOW);
#else
    bool success = m_cap.open(m_deviceIndex, cv::CAP_V4L2);
#endif

    if (success) {
        // Request preferred format
        if (m_preferW > 0) m_cap.set(cv::CAP_PROP_FRAME_WIDTH, m_preferW);
        if (m_preferH > 0) m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, m_preferH);
        if (m_preferFps > 0) m_cap.set(cv::CAP_PROP_FPS, m_preferFps);

        // Read actual format achieved
        m_actualW = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
        m_actualH = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        m_actualFps = m_cap.get(cv::CAP_PROP_FPS);
        
        if (m_actualFps <= 0.0) {
            m_actualFps = 30.0; // Fallback default
        }
    }
    return success;
}

bool CameraSource::read(cv::Mat& frame) {
    if (!m_cap.isOpened()) return false;
    return m_cap.read(frame);
}

void CameraSource::close() {
    if (m_cap.isOpened()) {
        m_cap.release();
    }
}

bool CameraSource::isOpened() const {
    return m_cap.isOpened();
}

int CameraSource::width() const {
    return m_actualW;
}

int CameraSource::height() const {
    return m_actualH;
}

double CameraSource::fps() const {
    return m_actualFps;
}

std::string CameraSource::description() const {
    return "Camera " + std::to_string(m_deviceIndex) + " (" + 
           std::to_string(m_actualW) + "x" + std::to_string(m_actualH) + ")";
}
