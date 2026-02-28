#pragma once
#include <vector>
#include <string>
#include <opencv2/core.hpp>

class LabelMap {
public:
    // 加载 txt（每行一个类别名称）或内置 COCO80
    bool loadFromFile(const std::string& path);
    void loadCOCO80();

    const std::string& nameOf(int classId) const;   // 越界返回 "unknown"
    int size() const;

    // 为每个类别生成固定颜色（BGR），用于可视化
    cv::Scalar colorOf(int classId) const;

private:
    std::vector<std::string>  m_names;
    mutable std::vector<cv::Scalar> m_colors;  // 惰性生成
};