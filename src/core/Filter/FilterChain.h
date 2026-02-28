#pragma once
#include "FilterBase.h"
#include <vector>
#include <memory>
#include <mutex>

class FilterChain {
public:
    using FilterPtr = std::shared_ptr<FilterBase>;

    // 追加滤镜到链末尾
    void append(FilterPtr filter);

    // 按 id 移除
    void remove(const std::string& filterId);

    // 调整顺序（移动索引 from → to）
    void move(std::size_t from, std::size_t to);

    // 清空
    void clear();

    // 顺序执行所有启用的滤镜（线程安全）
    cv::Mat process(const cv::Mat& src);

    // 获取指定 id 的滤镜（用于参数更新）
    FilterPtr find(const std::string& filterId);

    std::size_t size() const;

private:
    std::vector<FilterPtr> m_filters;
    mutable std::mutex     m_mutex;
};
