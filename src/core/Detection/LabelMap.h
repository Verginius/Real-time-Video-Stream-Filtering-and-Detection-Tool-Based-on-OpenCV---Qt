#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/core.hpp>

class LabelMap {
public:
    // 加载 txt（每行一个类别名称）
    inline bool loadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return false;
        m_names.clear();
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) m_names.push_back(line);
        }
        generateColors(); // 加载完后刷新颜色
        return true;
    }

    // 内置 COCO80 类别名
    inline void loadCOCO80() {
        m_names = { "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
                    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
                    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
                    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
                    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
                    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
                    "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
                    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
                    "hair drier", "toothbrush" };
        generateColors();
    }

    // 实现 nameOf：注意越界检查
    inline const std::string& nameOf(int classId) const {
        static const std::string unknown = "unknown";
        if (classId >= 0 && classId < static_cast<int>(m_names.size())) {
            return m_names[classId];
        }
        return unknown;
    }

    inline int size() const { return static_cast<int>(m_names.size()); }

    // 实现 colorOf
    inline cv::Scalar colorOf(int classId) const {
        if (classId >= 0 && classId < static_cast<int>(m_colors.size())) {
            return m_colors[classId];
        }
        return cv::Scalar(0, 255, 0); // 默认绿色
    }

private:
    // 内部私有方法，用于生成颜色
    inline void generateColors() {
        m_colors.clear();
        for (int i = 0; i < static_cast<int>(m_names.size()); ++i) {
            // 使用 ID 作为种子生成固定伪随机颜色
            int b = (i * 55) % 256;
            int g = (i * 123) % 256;
            int r = (i * 200) % 256;
            m_colors.push_back(cv::Scalar(b, g, r));
        }
    }

    std::vector<std::string> m_names;
    std::vector<cv::Scalar> m_colors; // 去掉 mutable，改为在加载时生成
};