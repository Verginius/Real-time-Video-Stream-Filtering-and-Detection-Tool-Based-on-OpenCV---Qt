#include "HistEqFilter.h"
#include <opencv2/imgproc.hpp>
#include <vector>

HistEqFilter::HistEqFilter(HistEqParams p) : m_params(p) {}

void HistEqFilter::setParams(HistEqParams p)
{
    std::lock_guard<std::mutex> lock(m_mu);
    
    if (p.clipLimit < 0.0) p.clipLimit = 0.0;
    if (p.tileGridW < 1) p.tileGridW = 1;
    if (p.tileGridH < 1) p.tileGridH = 1;
    
    m_params = p;
}

cv::Mat HistEqFilter::apply(const cv::Mat& src)
{
    if (!m_enabled || src.empty())
        return src.clone();

    HistEqParams p;
    {
        std::lock_guard<std::mutex> lock(m_mu);
        p = m_params;
    }

    if (src.channels() != 3 && src.channels() != 4) {
        cv::Mat dst;
        if (p.useCLAHE) {
            auto clahe = cv::createCLAHE(p.clipLimit, cv::Size(p.tileGridW, p.tileGridH));
            clahe->apply(src, dst);
        } else {
            cv::equalizeHist(src, dst);
        }
        cv::Mat bgr;
        cv::cvtColor(dst, bgr, cv::COLOR_GRAY2BGR);
        return bgr;
    }

    // 转 YCrCb，对 Y 通道做均衡化，再转回 BGR
    cv::Mat ycrcb;
    if (src.channels() == 4) {
        cv::Mat bgr;
        cv::cvtColor(src, bgr, cv::COLOR_BGRA2BGR);
        cv::cvtColor(bgr, ycrcb, cv::COLOR_BGR2YCrCb);
    } else {
        cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);
    }

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
