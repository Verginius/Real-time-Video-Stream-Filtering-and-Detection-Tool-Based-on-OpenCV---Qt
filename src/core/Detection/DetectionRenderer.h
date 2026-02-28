#pragma once
#include "Detection.h"
#include "LabelMap.h"
#include <opencv2/imgproc.hpp>

// 独立定义于类外，规避 GCC 嵌套结构体默认成员初始化器作为默认参数的限制
struct DetectionRenderStyle {
    int   boxThickness  = 2;
    float fontScale     = 0.55f;
    int   fontThickness = 1;
    bool  showScore     = true;
    bool  showLabel     = true;
};

class DetectionRenderer {
public:
    using Style = DetectionRenderStyle;

    explicit DetectionRenderer(const LabelMap& labels, Style style = Style{});

    // 在 frame 上原地绘制所有检测框（frame 须为可写副本）
    void render(cv::Mat& frame, const DetectionList& detections) const;

private:
    const LabelMap& m_labels;
    Style           m_style;
};