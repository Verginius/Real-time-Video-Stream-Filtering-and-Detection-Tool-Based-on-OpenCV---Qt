#pragma once
#include "DetectorBase.h"
#include "LabelMap.h"
#include <opencv2/dnn.hpp>
#include <mutex>

struct YOLOConfig {
    int    inputWidth   = 640;
    int    inputHeight  = 640;
    float  confThresh   = 0.50f;
    float  nmsThresh    = 0.45f;
    int    backendId    = cv::dnn::DNN_BACKEND_DEFAULT; // CUDA=cv::dnn::DNN_BACKEND_CUDA
    int    targetId     = cv::dnn::DNN_TARGET_CPU;      // CUDA=DNN_TARGET_CUDA
};

class YOLODetector : public DetectorBase {
public:
    explicit YOLODetector(YOLOConfig cfg = {});

    bool loadModel(const std::string& modelPath,
                   const std::string& labelsPath = "") override;
    DetectionList detect(const cv::Mat& frame)    override;
    bool  isLoaded()        const override;
    void  setConfThreshold(float t) override;
    float confThreshold()  const override;
    void  setNmsThreshold(float t) override;
    float nmsThreshold()   const override;

    // 设置后端（需重新加载模型）
    void setBackend(int backendId, int targetId);

    // 获取推理耗时（ms，最近一次）
    double lastInferenceMsec() const;

private:
    // --- 推理流程 ---
    cv::Mat   preprocess(const cv::Mat& frame);
    DetectionList postprocess(const std::vector<cv::Mat>& outputs,
                              const cv::Size& origSize);

    YOLOConfig      m_cfg;
    cv::dnn::Net    m_net;
    LabelMap        m_labels;
    bool            m_loaded = false;
    mutable std::mutex m_mutex;
    double          m_lastInfMs = 0.0;
    cv::Size        m_inputSize;
};