# 录制与导出模块详细设计

> **文档编号：** DD-EXP
> **所属阶段：** 第五阶段（第 6 周）
> **关联需求：** EXP-01 ~ EXP-03
> **状态：** 待实现

---

## 1. 模块职责

录制与导出模块接收处理后的帧（含检测叠加层），提供：
- **视频录制**：将处理后帧序列编码写入 MP4/AVI 文件（EXP-01）
- **截图保存**：将单帧保存为 PNG/JPEG（EXP-03）
- **检测结果导出**：将 `DetectionList` 序列追加写入 CSV 或 JSON 文件（EXP-02，P2）

---

## 2. 类结构

```
VideoRecorder            视频录制器（封装 cv::VideoWriter）
ScreenshotSaver          截图保存工具（静态方法）
ResultExporter           检测结果 CSV/JSON 导出器
```

---

## 3. 接口定义

### 3.1 VideoRecorder

```cpp
// src/core/Export/VideoRecorder.h
#pragma once
#include <opencv2/videoio.hpp>
#include <filesystem>
#include <mutex>
#include <string>

struct RecordConfig {
    std::filesystem::path outputDir;    // 输出目录
    std::string           prefix       = "record";
    int                   fourcc       = cv::VideoWriter::fourcc('m','p','4','v');
    double                fps          = 30.0;
    // width / height 从第一帧自动获取，无需手动设置
};

class VideoRecorder {
public:
    explicit VideoRecorder(RecordConfig cfg = {});
    ~VideoRecorder();

    // 开始录制（自动生成带时间戳的文件名）
    bool start();

    // 写入一帧（线程安全）
    void writeFrame(const cv::Mat& frame);

    // 停止录制，flush 并关闭文件
    // 返回最终输出文件路径
    std::filesystem::path stop();

    bool isRecording() const;

    // 已录制帧数
    std::size_t frameCount() const;

    // 已录制时长（秒）
    double durationSec() const;

private:
    std::string generateFilename() const;

    RecordConfig    m_cfg;
    cv::VideoWriter m_writer;
    bool            m_recording  = false;
    std::size_t     m_frameCount = 0;
    mutable std::mutex m_mutex;
    std::filesystem::path m_currentPath;
};
```

**关键实现要点：**

- `start()` 不立即打开 `cv::VideoWriter`，而是等待第一帧到来后才确定分辨率并调用 `m_writer.open()`。
- 文件命名格式：`record_20260301_143022.mp4`（使用 `std::chrono` + `<format>` 或 `strftime`）。
- `writeFrame()` 持有 `m_mutex`，允许工作线程写帧时 UI 线程调用 `stop()`。
- 编解码器优先选 `mp4v`（MP4 容器），可通过 `RecordConfig::fourcc` 改为 `XVID`（AVI）。

---

### 3.2 ScreenshotSaver

```cpp
// src/core/Export/ScreenshotSaver.h
#pragma once
#include <opencv2/core.hpp>
#include <filesystem>
#include <string>

class ScreenshotSaver {
public:
    enum class Format { PNG, JPEG };

    // 保存帧到指定目录，文件名自动带时间戳
    // 返回最终保存路径，失败返回空路径
    static std::filesystem::path save(
        const cv::Mat& frame,
        const std::filesystem::path& outputDir,
        Format fmt      = Format::PNG,
        int jpegQuality = 95);

    // 保存到指定完整路径
    static bool saveTo(
        const cv::Mat& frame,
        const std::filesystem::path& filePath,
        int jpegQuality = 95);

private:
    static std::string generateFilename(Format fmt);
};
```

**关键实现要点：**

- `cv::imwrite()` 支持 PNG（无损）和 JPEG（有损），通过 `cv::IMWRITE_JPEG_QUALITY` 控制质量。
- 文件名格式：`screenshot_20260301_143055_123.png`（精确到毫秒避免重名）。
- `outputDir` 不存在时自动 `std::filesystem::create_directories()`。

---

### 3.3 ResultExporter（P2）

```cpp
// src/core/Export/ResultExporter.h
#pragma once
#include "core/Detection/Detection.h"
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <mutex>

class ResultExporter {
public:
    enum class Format { CSV, JSON };

    explicit ResultExporter(std::filesystem::path filePath, Format fmt);
    ~ResultExporter();   // 自动 flush + close

    // 打开文件，写入 CSV 表头 / JSON 开头
    bool open();

    // 追加一帧的检测结果（含帧时间戳 ms）
    void appendFrame(std::int64_t timestampMsec,
                     const DetectionList& detections);

    // 手动关闭（也可依赖析构）
    void close();

    bool isOpen() const;

private:
    void writeCsvHeader();
    void writeCsvRow(std::int64_t ts, const Detection& d);
    void writeJsonFrameOpen(std::int64_t ts, std::size_t count);
    void writeJsonDetection(const Detection& d, bool last);
    void writeJsonFrameClose();
    void writeJsonFooter();

    std::filesystem::path m_path;
    Format                m_fmt;
    std::ofstream         m_ofs;
    bool                  m_firstFrame = true;
    mutable std::mutex    m_mutex;
};
```

**CSV 输出格式：**

```
timestamp_ms,frame_id,label,confidence,x,y,w,h
0,0,person,0.93,120,80,64,128
0,0,car,0.87,320,200,160,100
33,1,person,0.90,122,82,64,128
...
```

**JSON 输出格式：**

```json
[
  {
    "timestamp_ms": 0,
    "detections": [
      {"label": "person", "confidence": 0.93, "bbox": {"x":120,"y":80,"w":64,"h":128}},
      {"label": "car",    "confidence": 0.87, "bbox": {"x":320,"y":200,"w":160,"h":100}}
    ]
  },
  ...
]
```

---

## 4. 与 VideoController 的集成方式

```cpp
// VideoController 内部
class VideoController : public QObject {
    // ...
    VideoRecorder    m_recorder;
    ResultExporter*  m_exporter = nullptr;   // 可选，P2

public slots:
    void onRecordToggle() {
        if (!m_recorder.isRecording()) {
            m_recorder.start();
            emit recordingStateChanged(true);
        } else {
            auto path = m_recorder.stop();
            emit recordingStateChanged(false);
            emit recordingSaved(QString::fromStdString(path.string()));
        }
    }

    void onScreenshot() {
        auto path = ScreenshotSaver::save(
            m_lastProcessedFrame,
            QStandardPaths::writableLocation(
                QStandardPaths::PicturesLocation).toStdString());
        emit screenshotSaved(QString::fromStdString(path.string()));
    }

private:
    // 每帧处理完毕后
    void onFrameProcessed(const cv::Mat& processed,
                           const DetectionList& dets,
                           std::int64_t tsMsec) {
        if (m_recorder.isRecording())
            m_recorder.writeFrame(processed);
        if (m_exporter && m_exporter->isOpen())
            m_exporter->appendFrame(tsMsec, dets);
    }
};
```

---

## 5. 输出目录管理

| 类型 | 默认输出目录 |
|------|------------|
| 录制视频 | `~/Videos/RVSFDT/` （`QStandardPaths::MoviesLocation`） |
| 截图 | `~/Pictures/RVSFDT/` （`QStandardPaths::PicturesLocation`）|
| 检测结果 CSV/JSON | `~/Documents/RVSFDT/` （`QStandardPaths::DocumentsLocation`）|

用户可在设置对话框（P2）中修改默认目录。

---

## 6. 单元测试要点

```cpp
TEST(VideoRecorderTest, StartStop)      { /* start→写10帧→stop，文件存在且非空 */ }
TEST(VideoRecorderTest, FrameCount)     { /* 写 N 帧后 frameCount()==N */ }
TEST(ScreenshotSaverTest, SavePNG)      { /* 保存后文件存在，cv::imread 可读 */ }
TEST(ScreenshotSaverTest, SaveJPEG)     { /* JPEG 文件大小 < PNG */ }
TEST(ResultExporterTest, CsvHeader)     { /* 第一行含 "timestamp_ms" */ }
TEST(ResultExporterTest, JsonValid)     { /* 输出可被 JSON 解析器解析 */ }
```

---

## 7. 关键性能指标

| 指标 | 目标 |
|------|-----|
| `VideoRecorder::writeFrame()` 耗时（1080p） | < 5 ms（含编码） |
| 录制对主循环帧率影响 | < 1 FPS 损耗 |
| 截图保存耗时（1080p PNG） | < 50 ms（允许异步） |
| CSV 追加一行耗时 | < 0.1 ms |
