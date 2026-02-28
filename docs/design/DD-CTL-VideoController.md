# VideoController 核心控制器详细设计

> **文档编号：** DD-CTL
> **所属阶段：** 第一阶段骨架 + 全阶段持续完善
> **关联需求：** 全模块协调
> **状态：** 待实现

---

## 1. 模块职责

`VideoController` 是整个数据流的中枢，负责：
1. 持有并管理 `VideoSource`、`FilterChain`、`YOLODetector`、`VideoRecorder`、`ResultExporter` 等核心对象的生命周期。
2. 在独立工作线程中驱动帧循环（读帧 → 滤镜处理 → 检测 → 发帧）。
3. 作为 Qt 信号槽的中间人，将 GUI 的用户意图转为具体操作，并将数据结果回传给 GUI。

---

## 2. 架构概览

```
QThread (m_workerThread)
  └── VideoController (QObject, 移入工作线程)
        │
        ├── VideoSource*          输入帧
        ├── FilterChain           滤镜链处理
        ├── YOLODetector          检测推理（内部另开检测线程）
        ├── VideoRecorder         帧录制
        ├── ResultExporter        截图与结果导出
        ├── QTimer (m_frameTimer) 帧循环驱动
        └── FpsCounter            实时 FPS 统计
```

---

## 3. 类接口定义

```cpp
// src/core/VideoController.h
#pragma once

#include <QObject>
#include <QThread>
#include <QTimer>
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
```

---

## 4. 帧循环详细流程

```
QTimer::timeout (每 1000/fps ms)
    │
    ▼ doFrameLoop()
    │
    ├─ source->read(original)
    │      失败 → emit sourceError → stopFrameTimer
    │
    ├─ (可选) 帧率限流：FileSource 按原生 FPS 节流
    │
    ├─ processed = filterChain.process(original)
    │
    ├─ frameCounter++
    │   if (detectionEnabled && frameCounter % skipFrames == 0)
    │       latestDetections = detector.detect(processed)
    │       renderer.render(processed, latestDetections)  ← 叠加标注
    │   else
    │       renderer.render(processed, latestDetections)  ← 复用上次结果
    │
    ├─ if (recording) m_recorder.writeFrame(processed)
    │
    ├─ emit frameReady(original, processed, latestDetections)
    │
    ├─ fpsCounter.tick()
    │   if (每 30 帧) emit fpsUpdated(fpsCounter.current())
    │
    └─ if (FileSource) emit positionMsec(source->posMsec())
```

---

## 5. 线程模型

```
主线程（Qt Event Loop）
  └── MainWindow（GUI 操作、信号发送）
           │ QueuedConnection
           ▼
  工作线程（QThread m_workerThread）
  └── VideoController（frame loop, filter, detect, record）
           │ QueuedConnection
           ▼
  主线程
  └── MainWindow::onFrameReady()（渲染帧到 QLabel）
```

**初始化代码（MainWindow 构造函数）：**

```cpp
m_controller = new VideoController;
m_controller->moveToWorkerThread();

// GUI → Controller
connect(this, &MainWindow::openCameraRequested,
        m_controller, &VideoController::onOpenCamera,
        Qt::QueuedConnection);
// ... 所有控制信号类似连接

// Controller → GUI
connect(m_controller, &VideoController::frameReady,
        this, &MainWindow::onFrameReady,
        Qt::QueuedConnection);
// ...
```

---

## 6. 帧数据传递策略

### 问题
`cv::Mat` 是引用计数的浅拷贝，跨线程传递时若工作线程同时修改会导致竞争。

### 解决方案
在 `emit frameReady()` 前对原始帧和处理后帧各做一次 `.clone()`：

```cpp
emit frameReady(original.clone(), processed.clone(), latestDetections);
```

这保证 GUI 线程持有独立副本，工作线程可立即进入下一帧循环。  
代价：每帧约 2 × 6MB（1080p）= 12MB 拷贝。可优化为**双缓冲 `std::swap`** 方案减少拷贝次数（P2）。

---

## 7. 错误处理

| 场景 | 处理 |
|------|------|
| 视频源打开失败 | `openSource()` 不赋值 m_source，emit `sourceError` |
| 读帧失败（EOF / 断连）| `doFrameLoop()` 中 stopFrameTimer + emit `sourceError` |
| 模型加载失败 | emit `modelLoaded(false, errMsg)` |
| 录制写帧失败 | 停止录制 + emit `sourceError` |
| 截图路径无写权限 | emit `sourceError` |

---

## 8. FPS 限流细节

```cpp
void VideoController::startFrameTimer(double fps) {
    if (!m_frameTimer) {
        m_frameTimer = new QTimer(this);
        connect(m_frameTimer, &QTimer::timeout,
                this, &VideoController::doFrameLoop);
    }
    const int intervalMs = static_cast<int>(1000.0 / fps);
    m_frameTimer->start(intervalMs);
}
```

- `CameraSource`：fps 取 `source->fps()`（摄像头汇报值，通常 30）。
- `FileSource`：fps 取视频原生帧率，保证回放节奏正确。
- `ScreenSource`：fps 取用户设置值（默认 30）。

---

## 9. 多线程策略选型说明

> 本设计采用 **QTimer 驱动帧循环**，而非 `project plan.md` 风险对策中提及的**有界环形缓冲队列**方案。两者区别如下：

| 维度 | QTimer 驱动（本设计采用） | 有界环形缓冲队列 |
|------|--------------------------|------------------|
| 架构复杂度 | 低：单线程帧循环，无生产者/消费者同步 | 高：需要生产线程、消费线程、锁或无锁队列 |
| 延迟特性 | 帧生产即消费，端到端延迟极低 | 缓冲区引入额外 1–N 帧延迟 |
| 背压处理 | 自然背压：上一帧未处理完则下次 timeout 跳过 | 队列满时可选丢帧或阻塞生产者 |
| 适用场景 | 实时显示优先，允许偶发丢帧 | 高吞吐录制场景，追求帧完整性 |
| 代码可维护性 | 高：逻辑集中在 `doFrameLoop()` | 较低：需维护线程安全队列及两端生命周期 |

**结论：** 本项目以实时显示为首要目标，且通过跳帧推理已解决检测负载问题，故选择 QTimer 方案。针对无损录制场景（CLAHE + 背景差分同时开启时单帧处理可能超过 33ms），`VideoRecorder` 内部引入**独立 I/O 线程 + 有界写队列（上限 8 帧）**：工作线程仅做非阻塞入队（< 0.1ms），实际编码在 I/O 线程异步完成，队列满时丢弃最老帧并记录 `droppedFrames` 供状态栏告警。这在不改变整体帧循环架构的前提下消除了录制丢帧风险。

---

## 10. 关键性能指标

| 指标 | 目标 |
|------|-----|
| 帧循环总耗时（含滤镜，不含检测） | < 30 ms（1080p） |
| 帧循环总耗时（含全部 P0/P1 滤镜 + 跳帧检测）| < 33 ms |
| 工作线程 CPU 占用（1080p@30fps） | < 50% （单核等效） |
| 跨线程帧拷贝（.clone()）耗时 | < 5 ms（1080p） |
