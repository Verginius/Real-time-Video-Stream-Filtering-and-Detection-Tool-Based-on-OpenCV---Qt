#pragma once
#include <opencv2/core.hpp>
#include <string>

class FilterBase {
public:
    virtual ~FilterBase() = default;

    // 对 src 施加滤镜，返回处理后的帧
    virtual cv::Mat apply(const cv::Mat& src) = 0;

    // 滤镜唯一标识符（用于序列化/UI 匹配）
    virtual std::string id() const = 0;

    // 人类可读名称
    virtual std::string name() const = 0;

    // 是否启用（禁用时 apply 直接返回 src 的克隆）
    bool enabled() const { return m_enabled; }
    void setEnabled(bool e) { m_enabled = e; }

protected:
    bool m_enabled = true;
};
