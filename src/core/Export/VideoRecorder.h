#pragma once
#include <opencv2/videoio.hpp>
#include <filesystem>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <deque>
#include <atomic>
#include <string>

struct RecordConfig {
    std::filesystem::path outputDir;    // 输出目录
    std::string           prefix       = "record";
    int                   fourcc       = cv::VideoWriter::fourcc('m','p','4','v');
    double                fps          = 30.0;
    std::size_t           maxQueueSize = 8;   // 内部写队列上限（帧数）
    // width / height 从第一帧自动获取，无需手动设置
};

class VideoRecorder {
public:
    explicit VideoRecorder(RecordConfig cfg = {});
    ~VideoRecorder();

    // 开始录制（启动内部 I/O 线程，自动生成带时间戳的文件名）
    bool start();

    // 将帧入队（非阻塞，< 0.1ms；队列满时丢弃最老帧并递增 droppedFrames）
    void writeFrame(const cv::Mat& frame);

    // 停止录制：等待队列清空后 flush 并关闭文件，返回最终输出路径
    std::filesystem::path stop();

    bool isRecording() const;

    // 已入队帧数（含已编码 + 队列中待编码）
    std::size_t frameCount() const;

    // 已录制时长（秒，按入队帧数估算）
    double durationSec() const;

    // 因队列满而丢弃的帧数（供状态栏显示警告）
    std::size_t droppedFrames() const;

private:
    void ioThreadFunc();                      // I/O 线程主函数
    std::string generateFilename() const;

    RecordConfig    m_cfg;
    cv::VideoWriter m_writer;
    bool            m_recording  = false;
    std::size_t     m_frameCount = 0;
    std::atomic<std::size_t> m_droppedFrames{0};
    std::filesystem::path m_currentPath;

    // ── 内部写队列 ──────────────────────────────────────
    std::deque<cv::Mat>      m_queue;         // 有界帧队列（上限 maxQueueSize）
    mutable std::mutex       m_queueMutex;
    std::condition_variable  m_queueCv;
    std::atomic<bool>        m_stopIo{false};
    std::thread              m_ioThread;      // 独立 I/O 编码线程
};