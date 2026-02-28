#include "HistEqFilter.h"
#include <opencv2/imgproc.hpp>

HistEqFilter::HistEqFilter(HistEqParams p)
    : m_params(p)
{}

void HistEqFilter::setParams(HistEqParams p)
{
    std::lock_guard<std::mutex> lock(m_mu);
    m_params = p;
}

cv::Mat HistEqFilter::apply(const cv::Mat& src)
{
    if (!m_enabled)
        return src.clone();

    HistEqParams p;
    {
        std::lock_guard<std::mutex> lock(m_mu);
        p = m_params;
    }

    // 转 YCrCb，对 Y 通道做均衡化，再转回 BGR
    cv::Mat ycrcb;
    cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);

    std::vector<cv::Mat> channels;
    cv::split(ycrcb, channels);

    if (p.useCLAHE) {
        auto clahe = cv::createCLAHE(p.clipLimit,
                                     cv::Size(p.tileGridW, p.tileGridH));
        clahe->apply(channels[0], channels[0]);
    } else {
        cv::equalizeHist(channels[0], channels[0]);
    }

    cv::merge(channels, ycrcb);

    cv::Mat bgr;
    cv::cvtColor(ycrcb, bgr, cv::COLOR_YCrCb2BGR);
    return bgr;
}
