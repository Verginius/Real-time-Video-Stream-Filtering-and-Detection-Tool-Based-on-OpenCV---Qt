#include "ThresholdFilter.h"
#include <opencv2/imgproc.hpp>

ThresholdFilter::ThresholdFilter(ThresholdParams p)
    : m_params(p)
{}

void ThresholdFilter::setParams(ThresholdParams p)
{
    std::lock_guard<std::mutex> lock(m_mu);
    m_params = p;
}

cv::Mat ThresholdFilter::apply(const cv::Mat& src)
{
    if (!m_enabled)
        return src.clone();

    ThresholdParams p;
    {
        std::lock_guard<std::mutex> lock(m_mu);
        p = m_params;
    }

    cv::Mat gray, thresh, bgr;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    switch (p.type) {
    case ThresholdType::Fixed:
        cv::threshold(gray, thresh, p.value, 255, cv::THRESH_BINARY);
        break;
    case ThresholdType::Adaptive: {
        int bs = p.blockSize;
        if (bs % 2 == 0) bs++;
        if (bs < 3) bs = 3;
        cv::adaptiveThreshold(gray, thresh, 255,
            cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, bs, p.C);
        break;
    }
    case ThresholdType::Otsu:
        cv::threshold(gray, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        break;
    }

    cv::cvtColor(thresh, bgr, cv::COLOR_GRAY2BGR);
    return bgr;
}
