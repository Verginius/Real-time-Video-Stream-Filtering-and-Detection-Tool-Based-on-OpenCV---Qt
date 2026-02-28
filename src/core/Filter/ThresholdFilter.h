#pragma once
#include "FilterBase.h"
#include <mutex>

enum class ThresholdType { Fixed, Adaptive, Otsu };

struct ThresholdParams {
    ThresholdType type      = ThresholdType::Fixed;
    double        value     = 127.0;  // Fixed 阈值
    int           blockSize = 11;     // Adaptive 邻域大小（奇数）
    double        C         = 2.0;    // Adaptive 常数
};

class ThresholdFilter : public FilterBase {
public:
    explicit ThresholdFilter(ThresholdParams p = {});
    void setParams(ThresholdParams p);

    std::string id()   const override { return "threshold"; }
    std::string name() const override { return "二值化"; }
    cv::Mat apply(const cv::Mat& src) override;

private:
    ThresholdParams    m_params;
    mutable std::mutex m_mu;
};
