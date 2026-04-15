#include "CannyFilter.h"
#include <opencv2/imgproc.hpp>

CannyFilter::CannyFilter(CannyParams p) : m_params(p) {}

void CannyFilter::setParams(CannyParams p)
{
    std::lock_guard<std::mutex> lock(m_mu);
    
    // apertureSize must be 3, 5, or 7
    if (p.apertureSize != 3 && p.apertureSize != 5 && p.apertureSize != 7) {
        p.apertureSize = 3;
    }
    
    if (p.threshold1 < 0) p.threshold1 = 0;
    if (p.threshold2 < 0) p.threshold2 = 0;
    
    m_params = p;
}

cv::Mat CannyFilter::apply(const cv::Mat& src)
{
    if (!m_enabled || src.empty())
        return src.clone();

    CannyParams p;
    {
        std::lock_guard<std::mutex> lock(m_mu);
        p = m_params;
    }

    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else if (src.channels() == 4) {
        cv::cvtColor(src, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = src; // assume 1 channel
    }

    cv::Mat edges;
    cv::Canny(gray, edges, p.threshold1, p.threshold2, p.apertureSize, p.l2gradient);

    // Convert edges back to BGR so it matches the typical 3-channel pipeline
    cv::Mat bgrEdges;
    cv::cvtColor(edges, bgrEdges, cv::COLOR_GRAY2BGR);
    
    return bgrEdges;
}
