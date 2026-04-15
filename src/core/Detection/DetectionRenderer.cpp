#include "DetectionRenderer.h"
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <iomanip>

DetectionRenderer::DetectionRenderer(const LabelMap& labels, Style style)
    : m_labels(&labels), m_style(style) {
}

void DetectionRenderer::render(cv::Mat& frame, const DetectionList& detections) const {
    if (frame.empty() || detections.empty() || !m_labels) return;

    for (const auto& det : detections) {
        cv::Scalar color = m_labels->colorOf(det.classId);

        // Draw bounding box
        cv::rectangle(frame, det.bbox, color, m_style.boxThickness);

        // Draw label text if needed
        if (m_style.showLabel || m_style.showScore) {
            std::ostringstream textStream;
            if (m_style.showLabel) {
                textStream << det.label;
            }
            if (m_style.showLabel && m_style.showScore) {
                textStream << " ";
            }
            if (m_style.showScore) {
                textStream << std::fixed << std::setprecision(2) << det.confidence;
            }

            std::string text = textStream.str();
            
            // Calculate text size for background box
            int baseline = 0;
            cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 
                                               m_style.fontScale, m_style.fontThickness, &baseline);
            
            // Text origin
            cv::Point textOrigin(det.bbox.x, det.bbox.y > textSize.height ? det.bbox.y : det.bbox.y + textSize.height);
            
            // Draw text background
            cv::Rect textRect(textOrigin.x, textOrigin.y - textSize.height, textSize.width, textSize.height + baseline);
            cv::rectangle(frame, textRect, color, cv::FILLED);

            // Draw text
            cv::putText(frame, text, textOrigin, cv::FONT_HERSHEY_SIMPLEX, 
                        m_style.fontScale, cv::Scalar(255, 255, 255), m_style.fontThickness, cv::LINE_AA);
        }
    }
}
