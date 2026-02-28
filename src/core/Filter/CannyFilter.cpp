#include "CannyFilter.h"
#include <opencv2/imgproc.hpp>

CannyFilter::CannyFilter(CannyParams p)
    : m_params(p)
{}

void CannyFilter::setParams(CannyParams p)
{
    std::lock_guard<std::mutex> lock(m_mu);
    m_params = p;
}

cv::Mat CannyFilter::apply(const cv::Mat& src)
{
    if (!m_enabled)
        return src.clone();

    CannyParams p;
    {
        std::lock_guard<std::mutex> lock(m_mu);
        p = m_params;
    }

    cv::Mat gray, edges, bgr;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, p.threshold1, p.threshold2, p.apertureSize, p.l2gradient);
    cv::cvtColor(edges, bgr, cv::COLOR_GRAY2BGR);
    return bgr;
}
