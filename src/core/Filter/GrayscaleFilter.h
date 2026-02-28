#pragma once
#include "FilterBase.h"

class GrayscaleFilter : public FilterBase {
public:
    std::string id()   const override { return "grayscale"; }
    std::string name() const override { return "灰度化"; }
    cv::Mat apply(const cv::Mat& src) override;
};
