#include "VideoDisplay.h"

QImage VideoDisplay::matToQImage(const cv::Mat& mat) {
    if (mat.empty()) return QImage();
    // mat: BGR uint8 连续内存
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    // 构造 QImage 时传入 rgb.data 指针；rgb 在此函数返回前需保持存活
    // 故返回 QImage::copy() 或使用 QImage(data, w, h, step, Format_RGB888).copy()
    return QImage(rgb.data, rgb.cols, rgb.rows,
                  static_cast<int>(rgb.step),
                  QImage::Format_RGB888).copy();
}

void VideoDisplay::renderFrame(QLabel* label, const cv::Mat& frame) {
    if (frame.empty()) return;
    QImage img = matToQImage(frame);
    QPixmap pixmap = QPixmap::fromImage(img).scaled(
        label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(pixmap);
}