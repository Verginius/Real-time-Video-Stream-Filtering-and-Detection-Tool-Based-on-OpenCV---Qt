#pragma once
#include "FilterBase.h"
#include <mutex>

struct CannyParams {
    double threshold1   = 50.0;
    double threshold2   = 150.0;
    int    apertureSize = 3;    // Sobel 核，3/5/7
    bool   l2gradient   = false;
};

class CannyFilter : public FilterBase {
public:
    explicit CannyFilter(CannyParams p = {});
    void setParams(CannyParams p);

    std::string id()   const override { return "canny"; }
    std::string name() const override { return "Canny 边缘检测"; }
    cv::Mat apply(const cv::Mat& src) override;

private:
    CannyParams        m_params;
    mutable std::mutex m_mu;
};
