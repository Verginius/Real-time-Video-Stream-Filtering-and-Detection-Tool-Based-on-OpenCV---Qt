#pragma once
#include <opencv2/core.hpp>
#include <string>
#include <vector>

struct Detection {
    cv::Rect2f bbox;        // 像素坐标（浮点，保留子像素精度）
    int classId;
    float confidence;
    std::string label;
};

using DetectionList = std::vector<Detection>;
