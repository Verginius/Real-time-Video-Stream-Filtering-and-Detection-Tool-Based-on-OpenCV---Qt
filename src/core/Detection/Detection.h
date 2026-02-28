#pragma once
#include <opencv2/core.hpp>
#include <string>
#include <vector>

struct Detection {
    cv::Rect2f bbox;        // 像素坐标（浮点，保留子像素精度）
    int        classId;     // 类别 ID（COCO 0~79）
    float      confidence;  // 置信度 [0,1]
    std::string label;      // 类别名称字符串
};

using DetectionList = std::vector<Detection>;