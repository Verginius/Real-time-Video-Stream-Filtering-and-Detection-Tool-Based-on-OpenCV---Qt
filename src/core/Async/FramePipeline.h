#pragma once
#include <coroutine>
#include <memory>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <opencv2/core.hpp>
#include "../Detection/Detection.h"

class AsyncPipeline {
public:
    using FrameCallback = std::function<void(cv::Mat, cv::Mat, DetectionList)>;
    using FilterProcessor = std::function<cv::Mat(const cv::Mat&)>;
    using DetectorFunc = std::function<DetectionList(const cv::Mat&)>;

    AsyncPipeline();
    ~AsyncPipeline();

    void setFilterProcessor(FilterProcessor processor);
    void setDetector(DetectorFunc detector);
    void setCallback(FrameCallback callback);

    void start();
    void stop();
    bool isRunning() const;

    void enqueueFrame(cv::Mat frame);

private:
    struct FrameTask {
        cv::Mat original;
        cv::Mat processed;
        DetectionList detections;
    };

    struct AsyncFrame {
        cv::Mat frame;
        std::coroutine_handle<> continuation;
    };

    void processLoop();
    cv::Mat processFiltersAsync(const cv::Mat& src);
    DetectionList processDetectionAsync(const cv::Mat& src);

    FilterProcessor m_filterProcessor;
    DetectorFunc m_detector;
    FrameCallback m_callback;

    std::queue<AsyncFrame> m_frameQueue;
    std::mutex m_queueMutex;
    std::condition_variable m_queueCV;
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_stopRequested{false};

    std::thread m_processThread;
};

template<typename T>
class Task {
public:
    struct promise_type {
        T value;
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        template<typename U>
        std::suspend_always yield_value(U&& v) {
            value = std::forward<U>(v);
            return {};
        }
        template<typename U>
        void return_value(U&& v) {
            value = std::forward<U>(v);
        }
        Task get_return_object() { return Task{}; }
        void unhandled_exception() { std::terminate(); }
    };

    T get() { return {}; }
};

template<>
class Task<void> {
public:
    struct promise_type {
        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() {}
        Task get_return_object() { return Task{}; }
        void unhandled_exception() { std::terminate(); }
    };
};

template<typename T>
struct AwaitableTask {
    std::shared_ptr<T> result;
    std::coroutine_handle<> handle;

    bool await_ready() const { return false; }
    T await_resume() { return *result; }
    void await_suspend(std::coroutine_handle<> h) { handle = h; }
};
