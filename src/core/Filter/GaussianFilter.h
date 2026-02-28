#pragma once
#include "FilterBase.h"
#include <mutex>

struct GaussianParams {
    int    kernelSize = 5;   // 奇数，范围 [1, 31]
    double sigmaX     = 1.0; // 0 = 自动
    double sigmaY     = 0.0; // 0 = 同 sigmaX
};

class GaussianFilter : public FilterBase {
public:
    explicit GaussianFilter(GaussianParams p = {});
    void   setParams(GaussianParams p);
    GaussianParams params() const;

    std::string id()   const override { return "gaussian"; }
    std::string name() const override { return "高斯模糊"; }
    cv::Mat apply(const cv::Mat& src) override;

private:
    GaussianParams     m_params;
    mutable std::mutex m_mu;
};
