#pragma once
#include <QImage>
#include <QLabel>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class VideoDisplay {
public:
    // 将 cv::Mat (BGR) 转为 QPixmap 并设置到 label，保持纵横比缩放
    static void renderFrame(QLabel* label, const cv::Mat& frame);

private:
    // BGR cv::Mat → QImage (RGB888)，零拷贝版本（frame 需保持有效）
    static QImage matToQImage(const cv::Mat& mat);
};