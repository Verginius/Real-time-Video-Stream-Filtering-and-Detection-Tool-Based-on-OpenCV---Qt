#pragma once
#include "FilterBase.h"
#include <vector>
#include <memory>
#include <mutex>
#include <opencv2/core.hpp>

class FilterChain {
public:
    using FilterPtr = std::shared_ptr<FilterBase>;

    void append(FilterPtr filter);
    void remove(const std::string& filterId);
    void move(std::size_t from, std::size_t to);
    void clear();

    cv::Mat process(const cv::Mat& src);
    cv::Mat processParallel(const cv::Mat& src);

    FilterPtr find(const std::string& filterId);
    std::size_t size() const;

private:
    cv::Mat applyFilterParallel(const FilterPtr& filter, const cv::Mat& src);

    std::vector<FilterPtr> m_filters;
    mutable std::mutex     m_mutex;
};
