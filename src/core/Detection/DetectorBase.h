#pragma once
#include "Detection.h"
#include <opencv2/core.hpp>
#include <string>

class DetectorBase {
public:
    virtual ~DetectorBase() = default;

    // 加载模型（路径、标签文件路径）
    virtual bool loadModel(const std::string& modelPath,
                           const std::string& labelsPath = "") = 0;

    // 对 frame 执行推理，返回检测结果列表
    virtual DetectionList detect(const cv::Mat& frame) = 0;

    // 模型是否已加载
    virtual bool isLoaded() const = 0;

    // 设置置信度阈值（[0,1]）
    virtual void setConfThreshold(float thresh) = 0;
    virtual float confThreshold() const = 0;

    // 设置 NMS IOU 阈值
    virtual void setNmsThreshold(float thresh) = 0;
    virtual float nmsThreshold() const = 0;
};