#pragma once
#include "VideoSource.h"
#include <opencv2/videoio.hpp>
#include <filesystem>
#include <string>

class FileSource : public VideoSource {
public:
    explicit FileSource(std::filesystem::path filePath);

    bool   open()    override;
    bool   read(cv::Mat& frame) override;
    void   close()   override;
    bool   isOpened() const override;
    int    width()   const override;
    int    height()  const override;
    double fps()     const override;
    std::string description() const override;

    void   pause()  override;
    void   resume() override;
    bool   seek(double posMsec) override;
    double posMsec()     const override;
    double durationMsec() const override;

    // 步进一帧（需先 pause）
    bool stepForward();

private:
    std::filesystem::path m_path;
    cv::VideoCapture      m_cap;
    bool   m_paused   = false;
    double m_fps      = 0.0;
    int    m_w        = 0, m_h = 0;
    double m_duration = 0.0;  // ms
    cv::Mat m_lastFrame;
};
