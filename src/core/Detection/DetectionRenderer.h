#pragma once
#include "Detection.h"
#include "LabelMap.h"
#include <opencv2/imgproc.hpp>

class DetectionRenderer {
public:
    struct Style {
        int   boxThickness  = 2;
        float fontScale     = 0.55f;
        int   fontThickness = 1;
        bool  showScore     = true;
        bool  showLabel     = true;
    };

    explicit DetectionRenderer(const LabelMap& labels, Style style = {});

    // 在 frame 上原地绘制所有检测框（frame 须为可写副本）
    void render(cv::Mat& frame, const DetectionList& detections) const;

private:
    const LabelMap& m_labels;
    Style           m_style;
};