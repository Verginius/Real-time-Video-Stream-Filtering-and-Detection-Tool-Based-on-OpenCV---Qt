#pragma once
#include "FilterBase.h"
#include <mutex>

struct HistEqParams {
    bool   useCLAHE  = true;
    double clipLimit = 2.0;
    int    tileGridW = 8;
    int    tileGridH = 8;
};

class HistEqFilter : public FilterBase {
public:
    explicit HistEqFilter(HistEqParams p = {});
    void setParams(HistEqParams p);

    std::string id()   const override { return "histeq"; }
    std::string name() const override { return "CLAHE 均衡化"; }
    cv::Mat apply(const cv::Mat& src) override;

private:
    HistEqParams       m_params;
    mutable std::mutex m_mu;
};
