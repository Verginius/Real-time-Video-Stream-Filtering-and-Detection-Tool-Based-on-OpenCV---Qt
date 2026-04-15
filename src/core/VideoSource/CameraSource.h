#pragma once
#include "VideoSource.h"
#include <opencv2/videoio.hpp>
#include <vector>

struct CameraInfo {
    int    index;       // cv::VideoCapture 设备索引
    std::string name;   // 设备名称（平台相关）
};

class CameraSource : public VideoSource {
public:
    // 枚举当前系统可用摄像头（最多探测 maxProbe 个索引）
    static std::vector<CameraInfo> enumerateDevices(int maxProbe = 8);

    explicit CameraSource(int deviceIndex = 0,
                          int preferWidth  = 1280,
                          int preferHeight = 720,
                          int preferFps    = 30);

    bool   open()    override;
    bool   read(cv::Mat& frame) override;
    void   close()   override;
    bool   isOpened() const override;
    int    width()   const override;
    int    height()  const override;
    double fps()     const override;
    std::string description() const override;

private:
    int    m_deviceIndex;
    int    m_preferW, m_preferH, m_preferFps;
    cv::VideoCapture m_cap;
    double m_actualFps  = 0.0;
    int    m_actualW    = 0;
    int    m_actualH    = 0;
};
