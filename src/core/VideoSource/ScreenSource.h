#pragma once
#include "VideoSource.h"
#include <QRect>
#include <QImage>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <chrono>

class ScreenSource : public VideoSource {
public:
    // region: 屏幕坐标矩形；fps: 期望捕获帧率
    explicit ScreenSource(QRect region, double fps = 30.0);

    bool   open()    override;
    bool   read(cv::Mat& frame) override;
    void   close()   override;
    bool   isOpened() const override;
    int    width()   const override;
    int    height()  const override;
    double fps()     const override;
    std::string description() const override;

    void setRegion(QRect region);   // 动态调整捕获区域

private:
    cv::Mat qImageToMat(const QImage& img);

    QRect  m_region;
    double m_fps;
    bool   m_opened = false;
    
    QScreen* m_screen = nullptr;
    
    std::chrono::steady_clock::time_point m_lastReadTime;
    double m_readIntervalMs;
};
