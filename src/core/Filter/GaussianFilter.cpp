#include "GaussianFilter.h"
#include <opencv2/imgproc.hpp>
#include <algorithm>

GaussianFilter::GaussianFilter(GaussianParams p) : m_params(p) {}

void GaussianFilter::setParams(GaussianParams p)
{
    std::lock_guard<std::mutex> lock(m_mu);
    // Ensure kernel size is valid: odd and >= 1
    if (p.kernelSize < 1) p.kernelSize = 1;
    if (p.kernelSize % 2 == 0) p.kernelSize++;
    
    // clamp kernel size to avoid huge allocations/performance drops
    p.kernelSize = std::min(p.kernelSize, 31);
    
    // Ensure sigmas are non-negative
    if (p.sigmaX < 0) p.sigmaX = 0;
    if (p.sigmaY < 0) p.sigmaY = 0;

    m_params = p;
}

GaussianParams GaussianFilter::params() const
{
    std::lock_guard<std::mutex> lock(m_mu);
    return m_params;
}

cv::Mat GaussianFilter::apply(const cv::Mat& src)
{
    if (!m_enabled || src.empty())
        return src.clone();

    GaussianParams p = params();
    
    if (p.kernelSize <= 1 && p.sigmaX <= 0) {
        return src.clone();
    }

    cv::Mat dst;
    cv::GaussianBlur(src, dst, cv::Size(p.kernelSize, p.kernelSize), p.sigmaX, p.sigmaY);
    return dst;
}
