#include "GrayscaleFilter.h"
#include <opencv2/imgproc.hpp>

cv::Mat GrayscaleFilter::apply(const cv::Mat& src)
{
    if (!m_enabled)
        return src.clone();

    cv::Mat gray, bgr;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, bgr, cv::COLOR_GRAY2BGR);
    return bgr;
}
