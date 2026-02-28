#include "GaussianFilter.h"
#include <opencv2/imgproc.hpp>

GaussianFilter::GaussianFilter(GaussianParams p)
    : m_params(p)
{}

void GaussianFilter::setParams(GaussianParams p)
{
    std::lock_guard<std::mutex> lock(m_mu);
    m_params = p;
}

GaussianParams GaussianFilter::params() const
{
    std::lock_guard<std::mutex> lock(m_mu);
    return m_params;
}

cv::Mat GaussianFilter::apply(const cv::Mat& src)
{
    if (!m_enabled)
        return src.clone();

    GaussianParams p;
    {
        std::lock_guard<std::mutex> lock(m_mu);
        p = m_params;
    }

    // 强制奇数化
    int k = p.kernelSize;
    if (k % 2 == 0) k++;
    if (k < 1) k = 1;

    cv::Mat dst;
    cv::GaussianBlur(src, dst, cv::Size(k, k), p.sigmaX, p.sigmaY);
    return dst;
}
