#include "ScreenSource.h"
#include <opencv2/imgproc.hpp>
#include <thread>

ScreenSource::ScreenSource(QRect region, double fps)
    : m_region(region), m_fps(fps > 0 ? fps : 30.0)
{
    m_readIntervalMs = 1000.0 / m_fps;
}

bool ScreenSource::open() {
    m_screen = QGuiApplication::primaryScreen();
    if (!m_screen) {
        return false;
    }
    
    // Ensure the region is valid
    if (!m_region.isValid()) {
        m_region = m_screen->geometry();
    }
    
    m_opened = true;
    m_lastReadTime = std::chrono::steady_clock::now();
    return true;
}

bool ScreenSource::read(cv::Mat& frame) {
    if (!m_opened || !m_screen) return false;
    
    // Enforce FPS limit for screen capture
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = now - m_lastReadTime;
    
    if (elapsed.count() < m_readIntervalMs) {
        std::this_thread::sleep_for(std::chrono::milliseconds(
            static_cast<long long>(m_readIntervalMs - elapsed.count())));
    }
    
    // Grab the window
    QPixmap pixmap = m_screen->grabWindow(0, m_region.x(), m_region.y(), 
                                          m_region.width(), m_region.height());
    if (pixmap.isNull()) return false;
    
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    frame = qImageToMat(img);
    
    m_lastReadTime = std::chrono::steady_clock::now();
    return !frame.empty();
}

void ScreenSource::close() {
    m_opened = false;
    m_screen = nullptr;
}

bool ScreenSource::isOpened() const {
    return m_opened;
}

int ScreenSource::width() const {
    return m_region.width();
}

int ScreenSource::height() const {
    return m_region.height();
}

double ScreenSource::fps() const {
    return m_fps;
}

std::string ScreenSource::description() const {
    return "Screen (" + std::to_string(m_region.width()) + "x" + std::to_string(m_region.height()) + ")";
}

void ScreenSource::setRegion(QRect region) {
    if (region.isValid()) {
        m_region = region;
    }
}

cv::Mat ScreenSource::qImageToMat(const QImage& img) {
    // Construct cv::Mat pointing to QImage data (no deep copy yet)
    cv::Mat mat(img.height(), img.width(), CV_8UC3,
                const_cast<uchar*>(img.bits()), img.bytesPerLine());
    
    cv::Mat result;
    // Convert RGB to BGR which also makes a deep copy
    cv::cvtColor(mat, result, cv::COLOR_RGB2BGR);
    return result;
}
