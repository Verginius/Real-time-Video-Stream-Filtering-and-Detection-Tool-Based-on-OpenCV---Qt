#pragma once

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QRect>
#include <memory>
#include <atomic>

#include "VideoSource/VideoSource.h"   // VideoSource 纯虚基类
#include "Filter/FilterChain.h"
#include "Detection/YOLODetector.h"
#include "Detection/DetectionRenderer.h"
#include "Export/VideoRecorder.h"
#include "Export/ResultExporter.h"

class VideoController : public QObject {
    Q_OBJECT

public:
    explicit VideoController(QObject* parent = nullptr);
    ~VideoController();

    // 在 QThread 中启动（由 MainWindow 调用）
    void moveToWorkerThread();

signals:
    // ──── 向 GUI 回传数据 ────
    // 每帧处理完成后发射（跨线程 QueuedConnection）
    void frameReady(cv::Mat original, cv::Mat processed, DetectionList detections);
    void fpsUpdated(double fps);
    void resolutionChanged(int width, int height);
    void durationMsec(double ms);       // 仅 FileSource 有效
    void positionMsec(double ms);       // 当前播放位置
    void sourceOpened(const QString& description);
    void sourceClosed();
    void sourceError(const QString& message);
    void recordingStateChanged(bool recording);
    void recordingSaved(const QString& path);
    void screenshotSaved(const QString& path);
    void modelLoaded(bool success, const QString& message);

public slots:
    // ──── 接收 GUI 指令 ────
    void onOpenCamera(int deviceIndex);
    void onOpenFile(const QString& path);
    void onOpenScreen(QRect region, double fps);
    void onPlayPause();
    void onStop();
    void onSeek(double posMsec);

    // 滤镜
    void onSetFilterEnabled(const QString& filterId, bool enabled);
    void onSetGaussianParams(int kernelSteps, double sigma);
    void onSetCannyParams(double thresh1, double thresh2);
    void onSetThresholdParams(int type, int value);
    void onSetHistEqParams(bool useClahe, double clipLimit);
    void onSetSharpenParams(double strength, double sigma);
    void onSetBgSubParams(int algo);

    // 检测
    void onLoadModel(const QString& modelPath, const QString& labelsPath);
    void onSetDetectionEnabled(bool enabled);
    void onSetConfThreshold(float thresh);
    void onSetNmsThreshold(float thresh);
    void onSetSkipFrames(int n);

    // 导出
    void onScreenshot();                                 // 触发截图
    void onExportDetections(const QString& format);       // "csv" / "json"
    void onRecordToggle();
    void onSetRecordOutputDir(const QString& dir);

private slots:
    void doFrameLoop();   // 由 m_frameTimer 触发

private:
    void openSource(std::unique_ptr<VideoSource> source);
    void closeSource();
    void startFrameTimer(double fps);
    void stopFrameTimer();

    // ──── 核心对象 ────
    std::unique_ptr<VideoSource> m_source;
    FilterChain                  m_filterChain;
    YOLODetector                 m_detector;
    DetectionRenderer            m_renderer;
    VideoRecorder                m_recorder;
    ResultExporter               m_exporter;

    // ──── 帧循环 ────
    QTimer*          m_frameTimer   = nullptr;
    QThread*         m_workerThread = nullptr;

    // ──── 检测跳帧 ────
    int              m_skipFrames   = 3;
    int              m_frameCounter = 0;
    std::atomic<bool> m_detectionEnabled{false};

    // 最新检测结果（原子替换，供跳帧期间复用）
    std::mutex   m_detMutex;
    DetectionList m_latestDetections;

    // ──── 状态 ────
    bool  m_paused    = false;
    bool  m_recording = false;
    cv::Mat m_lastOrigFrame;
    cv::Mat m_lastProcessedFrame;

    // ──── FPS 统计 ────
    struct FpsCounter {
        void tick();
        double current() const;
    private:
        std::deque<std::chrono::steady_clock::time_point> m_times;
    } m_fpsCounter;
};