#include "GrayscaleFilter.h"
#include <opencv2/imgproc.hpp>

cv::Mat GrayscaleFilter::apply(const cv::Mat& src)
{
    if (!m_enabled || src.empty())
        return src.clone();

    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else if (src.channels() == 4) {
        cv::cvtColor(src, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = src.clone();
    }
    
    // In order to not break the pipeline for filters expecting 3 channels, 
    // we convert back to BGR.
    cv::Mat bgr;
    if (gray.channels() == 1) {
        cv::cvtColor(gray, bgr, cv::COLOR_GRAY2BGR);
    } else {
        bgr = gray;
    }

    return bgr;
}
