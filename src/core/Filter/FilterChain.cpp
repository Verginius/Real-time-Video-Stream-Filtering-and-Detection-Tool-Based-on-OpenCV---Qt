#include "FilterChain.h"
#include <algorithm>

void FilterChain::append(FilterPtr filter)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filters.push_back(std::move(filter));
}

void FilterChain::remove(const std::string& filterId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filters.erase(
        std::remove_if(m_filters.begin(), m_filters.end(),
            [&filterId](const FilterPtr& f) { return f->id() == filterId; }),
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
    cv::Mat frame = src;
    for (const auto& f : m_filters) {
        if (f->enabled()) {
            frame = f->apply(frame);
        }
    }
    return frame;
}

FilterChain::FilterPtr FilterChain::find(const std::string& filterId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& f : m_filters) {
        if (f->id() == filterId)
            return f;
    }
    return nullptr;
}

std::size_t FilterChain::size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_filters.size();
}
