#pragma once
#include <opencv2/core.hpp>
#include <string>

class VideoSource {
public:
    virtual ~VideoSource() = default;

    // 打开输入源；失败返回 false
    virtual bool open() = 0;

    // 读取下一帧；无帧可读或已到末尾返回 false
    virtual bool read(cv::Mat& frame) = 0;

    // 释放资源
    virtual void close() = 0;

    // 是否已处于打开状态
    virtual bool isOpened() const = 0;

    // 当前分辨率
    virtual int width()  const = 0;
    virtual int height() const = 0;

    // 视频源原生 FPS（摄像头/屏幕捕获返回期望值）
    virtual double fps() const = 0;

    // 人类可读的输入源描述
    virtual std::string description() const = 0;

    // --- 可选控制（子类按需重写） ---
    virtual void pause()  {}      // 暂停
    virtual void resume() {}      // 继续
    virtual bool seek(double /*posMsec*/) { return false; }   // 跳转（仅 FileSource）
    virtual double posMsec() const { return 0.0; }            // 当前位置（ms）
    virtual double durationMsec() const { return 0.0; }       // 总时长（ms）
};