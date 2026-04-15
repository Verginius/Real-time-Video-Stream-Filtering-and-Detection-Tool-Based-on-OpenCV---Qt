#include "ThresholdFilter.h"
#include <opencv2/imgproc.hpp>
#include <algorithm>

ThresholdFilter::ThresholdFilter(ThresholdParams p) : m_params(p) {}

void ThresholdFilter::setParams(ThresholdParams p)
{
    std::lock_guard<std::mutex> lock(m_mu);
    
    // clamp value between 0 and 255
    p.value = std::max(0.0, std::min(255.0, p.value));
    
    // blockSize for adaptive must be odd and > 1
    if (p.blockSize <= 1) p.blockSize = 3;
    if (p.blockSize % 2 == 0) p.blockSize++;
    
    m_params = p;
}

cv::Mat ThresholdFilter::apply(const cv::Mat& src)
{
    if (!m_enabled || src.empty())
        return src.clone();

    ThresholdParams p;
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
        gray = src;
    }

    cv::Mat dst;
    if (p.type == ThresholdType::Adaptive) {
        cv::adaptiveThreshold(gray, dst, 255, 
            cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
            cv::THRESH_BINARY, p.blockSize, p.C);
    } else if (p.type == ThresholdType::Otsu) {
        cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    } else { // Fixed
        cv::threshold(gray, dst, p.value, 255, cv::THRESH_BINARY);
    }

    cv::Mat bgr;
    cv::cvtColor(dst, bgr, cv::COLOR_GRAY2BGR);
    return bgr;
}
