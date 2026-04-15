#include "FilterChain.h"
#include <algorithm>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

void FilterChain::append(FilterPtr filter)
{
    if (!filter) return;
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filters.push_back(std::move(filter));
}

void FilterChain::remove(const std::string& filterId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filters.erase(
        std::remove_if(m_filters.begin(), m_filters.end(),
            [&filterId](const FilterPtr& f) { return f && f->id() == filterId; }),
        m_filters.end());
}

void FilterChain::move(std::size_t from, std::size_t to)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (from >= m_filters.size() || to >= m_filters.size() || from == to)
        return;

    FilterPtr filter = m_filters[from];
    m_filters.erase(m_filters.begin() + static_cast<std::ptrdiff_t>(from));
    m_filters.insert(m_filters.begin() + static_cast<std::ptrdiff_t>(to), std::move(filter));
}

void FilterChain::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filters.clear();
}

cv::Mat FilterChain::process(const cv::Mat& src)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (src.empty()) {
        return src;
    }

    cv::Mat frame = src.clone();
    for (const auto& f : m_filters) {
        if (f && f->enabled()) {
            frame = f->apply(frame);
        }
    }
    return frame;
}

FilterChain::FilterPtr FilterChain::find(const std::string& filterId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& f : m_filters) {
        if (f && f->id() == filterId)
            return f;
    }
    return nullptr;
}

std::size_t FilterChain::size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_filters.size();
}

cv::Mat FilterChain::processParallel(const cv::Mat& src)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (src.empty()) {
        return src;
    }

    cv::Mat frame = src.clone();
    for (const auto& f : m_filters) {
        if (f && f->enabled()) {
            frame = applyFilterParallel(f, frame);
        }
    }
    return frame;
}

cv::Mat FilterChain::applyFilterParallel(const FilterPtr& filter, const cv::Mat& src)
{
    if (src.empty() || !filter) return src.clone();

    const int numThreads = 4;
    const int height = src.rows;
    const int width = src.cols;
    const int chunkHeight = height / numThreads;

    std::vector<cv::Mat> results(numThreads);

    cv::parallel_for_(cv::Range(0, numThreads), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            int startRow = i * chunkHeight;
            int endRow = (i == numThreads - 1) ? height : (i + 1) * chunkHeight;
            
            cv::Rect roi(0, startRow, width, endRow - startRow);
            cv::Mat chunk = src(roi).clone();
            results[i] = filter->apply(chunk);
        }
    });

    cv::Mat dst;
    cv::vconcat(results, dst);
    return dst;
}
