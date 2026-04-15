#pragma once
#include "DetectionBackend.h"
#include "Detection/Detection.h"

#ifdef HAVE_TENSORRT
#include <NvInfer.h>
#include <cuda_runtime_api.h>
#include <memory>
#include <string>
#include <mutex>
#include <vector>
#include <iostream>

class TensorRTBackend : public DetectionBackend {
private:
    class Logger : public nvinfer1::ILogger {
    void log(nvinfer1::ILogger::Severity severity, const char* msg) noexcept override {
        if (severity == nvinfer1::ILogger::Severity::kERROR) {
            std::cerr << "TensorRT ERROR: " << msg << std::endl;
        }
    }
    } m_logger;

public:
    TensorRTBackend();
    ~TensorRTBackend() override;

    bool loadModel(const std::string& modelPath,
                   const std::string& labelsPath = "") override;
    DetectionList detect(const cv::Mat& frame) override;
    bool isLoaded() const override;
    void setConfThreshold(float thresh) override;
    float confThreshold() const override;
    void setNmsThreshold(float thresh) override;
    float nmsThreshold() const override;
    double lastInferenceMsec() const override;

private:
    cv::Mat preprocess(const cv::Mat& frame);
    DetectionList postprocess(const float* output, int outputSize, const cv::Size& origSize);

    std::unique_ptr<nvinfer1::IRuntime> m_runtime;
    std::unique_ptr<nvinfer1::ICudaEngine> m_engine;
    std::unique_ptr<nvinfer1::IExecutionContext> m_context;

    std::string m_modelPath;
    std::string m_enginePath;
    bool m_loaded = false;
    float m_confThresh = 0.5f;
    float m_nmsThresh = 0.45f;
    double m_lastInfMs = 0.0;
    cv::Size m_inputSize = {640, 640};

    mutable std::mutex m_mutex;
    std::vector<float> m_inputBuffer;
    std::vector<float> m_outputBuffer;

    void* m_cudaStream = nullptr;
    void* m_deviceBuffer = nullptr;

    bool buildEngine(const std::string& onnxPath, const std::string& enginePath);
    bool loadEngine(const std::string& enginePath);
};

#endif
