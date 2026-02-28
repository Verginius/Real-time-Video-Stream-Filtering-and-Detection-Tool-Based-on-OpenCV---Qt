# 视频输入模块详细设计

> **文档编号：** DD-VIN
> **所属阶段：** 第二阶段（第 2 周）
> **关联需求：** VIN-01 ~ VIN-05
> **状态：** 待实现

---

## 1. 模块职责

视频输入模块负责从多种输入源（摄像头、视频文件、屏幕捕获）以统一接口向上层提供原始 `cv::Mat` 帧序列，并汇报当前帧率与分辨率。

---

## 2. 类结构

```
VideoSource              (纯虚抽象基类)
├── CameraSource         摄像头输入
├── FileSource           本地视频文件
└── ScreenSource         屏幕区域捕获
```

---

## 3. 接口定义

### 3.1 VideoSource

```cpp
// src/core/VideoSource/VideoSource.h
#pragma once
#include <opencv2/core.hpp>
#include <string>

class VideoSource {
public:
    virtual ~VideoSource() = default;

    // 打开输入源；失败返回 false
    virtual bool open() = 0;

    // 读取下一帧；无帧可读或已到末尾返回 false
    virtual bool read(cv::Mat& frame) = 0;

    // 释放资源
    virtual void close() = 0;

    // 是否已处于打开状态
    virtual bool isOpened() const = 0;

    // 当前分辨率
    virtual int width()  const = 0;
    virtual int height() const = 0;

    // 视频源原生 FPS（摄像头/屏幕捕获返回期望值）
    virtual double fps() const = 0;

    // 人类可读的输入源描述
    virtual std::string description() const = 0;

    // --- 可选控制（子类按需重写） ---
    virtual void pause()  {}      // 暂停
    virtual void resume() {}      // 继续
    virtual bool seek(double /*posMsec*/) { return false; }   // 跳转（仅 FileSource）
    virtual double posMsec() const { return 0.0; }            // 当前位置（ms）
    virtual double durationMsec() const { return 0.0; }       // 总时长（ms）
};
```

---

### 3.2 CameraSource

**职责：** 枚举并打开本地摄像头设备（USB / 内置），持续捕获帧。

```cpp
// src/core/VideoSource/CameraSource.h
#pragma once
#include "VideoSource.h"
#include <opencv2/videoio.hpp>
#include <vector>

struct CameraInfo {
    int    index;       // cv::VideoCapture 设备索引
    std::string name;   // 设备名称（平台相关）
};

class CameraSource : public VideoSource {
public:
    // 枚举当前系统可用摄像头（最多探测 maxProbe 个索引）
    static std::vector<CameraInfo> enumerateDevices(int maxProbe = 8);

    explicit CameraSource(int deviceIndex = 0,
                          int preferWidth  = 1280,
                          int preferHeight = 720,
                          int preferFps    = 30);

    bool   open()    override;
    bool   read(cv::Mat& frame) override;
    void   close()   override;
    bool   isOpened() const override;
    int    width()   const override;
    int    height()  const override;
    double fps()     const override;
    std::string description() const override;

private:
    int    m_deviceIndex;
    int    m_preferW, m_preferH, m_preferFps;
    cv::VideoCapture m_cap;
    double m_actualFps  = 0.0;
    int    m_actualW    = 0;
    int    m_actualH    = 0;
};
```

**关键实现要点：**
- `enumerateDevices()` 顺序尝试索引 0…maxProbe−1，用 `cv::VideoCapture::isOpened()` 判断有效性，Windows 上可通过 DirectShow / MFEnumerator 获取友好名称。
- `open()` 使用 `cv::CAP_DSHOW`（Windows）或 `cv::CAP_V4L2`（Linux）后端以降低延迟。
- `read()` 直接调用 `m_cap.read(frame)`，调用方负责节流（由 VideoController 的帧定时器管理）。

---

### 3.3 FileSource

**职责：** 打开本地视频文件，支持暂停、继续、跳转、逐帧步进。

```cpp
// src/core/VideoSource/FileSource.h
#pragma once
#include "VideoSource.h"
#include <opencv2/videoio.hpp>
#include <filesystem>

class FileSource : public VideoSource {
public:
    explicit FileSource(std::filesystem::path filePath);

    bool   open()    override;
    bool   read(cv::Mat& frame) override;
    void   close()   override;
    bool   isOpened() const override;
    int    width()   const override;
    int    height()  const override;
    double fps()     const override;
    std::string description() const override;

    void   pause()  override;
    void   resume() override;
    bool   seek(double posMsec) override;
    double posMsec()     const override;
    double durationMsec() const override;

    // 步进一帧（需先 pause）
    bool stepForward();

private:
    std::filesystem::path m_path;
    cv::VideoCapture      m_cap;
    bool   m_paused   = false;
    double m_fps      = 0.0;
    int    m_w        = 0, m_h = 0;
    double m_duration = 0.0;  // ms
};
```

**关键实现要点：**
- `read()` 在 `m_paused == true` 时直接返回最后一帧（缓存一份），不推进读头。
- `seek()` 通过 `m_cap.set(cv::CAP_PROP_POS_MSEC, ms)` 实现。
- `stepForward()` 先 `m_cap.grab()` 再 `m_cap.retrieve(frame)`，适用于逐帧调试。
- 文件格式支持：MP4（H.264）、AVI、MKV、MOV；依赖 OpenCV 编译时的 FFmpeg 支持。

---

### 3.4 ScreenSource

**职责：** 捕获屏幕指定区域（`QRect`），以固定帧率提供帧。

```cpp
// src/core/VideoSource/ScreenSource.h
#pragma once
#include "VideoSource.h"
#include <QRect>
#include <QImage>

class ScreenSource : public VideoSource {
public:
    // region: 屏幕坐标矩形；fps: 期望捕获帧率
    explicit ScreenSource(QRect region, double fps = 30.0);

    bool   open()    override;
    bool   read(cv::Mat& frame) override;
    void   close()   override;
    bool   isOpened() const override;
    int    width()   const override;
    int    height()  const override;
    double fps()     const override;
    std::string description() const override;

    void setRegion(QRect region);   // 动态调整捕获区域

private:
    cv::Mat qImageToMat(const QImage& img);

    QRect  m_region;
    double m_fps;
    bool   m_opened = false;
};
```

**关键实现要点：**
- **Windows：** 使用 `QScreen::grabWindow()` 方案（跨平台，但有 DRM 限制），或通过 DXGI Desktop Duplication API 以获得低延迟/高帧率（P2 实现）。
- **Linux：** 使用 `XGetImage` / `XShmGetImage`（X11）或 `portal` screenshotAPI（Wayland）。
- `qImageToMat()` 将 `QImage::Format_RGB32` 转为 `cv::Mat` BGR：先 `QImage::Format_RGB888` 再 `cv::cvtColor(RGB→BGR)`，零拷贝版本可利用 `QImage` data指针直接构造 `cv::Mat header`。

---

## 4. 线程模型

```
VideoController (主循环线程)
   └─ 定时器（QTimer / 线程睡眠）
        └─ VideoSource::read(frame)  ← 阻塞调用（< 1 帧时间）
             └─ 发射 frameReady(frame) 到处理管道
```

`VideoSource` 本身**不持有线程**，由 `VideoController` 在工作线程中驱动读帧，通过 Qt 信号槽将帧传递给处理管道。

---

## 5. 错误处理

| 场景 | 处理方式 |
|------|---------|
| 摄像头被占用 | `open()` 返回 `false`，上层弹出提示框 |
| 文件不存在/格式不支持 | 同上 |
| 读帧失败（设备断连） | `read()` 返回 `false`，VideoController 发出 `sourceError` 信号 |
| 屏幕捕获权限不足 | `open()` 返回 `false`，提示用户授权 |

---

## 6. 单元测试要点（test_video_source.cpp）

```cpp
// 示例测试用例骨架
TEST(FileSourceTest, OpenValidFile)    { /* 打开有效 MP4，isOpened()=true */ }
TEST(FileSourceTest, ReadFrames)       { /* 连续读取 10 帧，均非空 */ }
TEST(FileSourceTest, SeekAndRead)      { /* seek 到 5000ms，验证位置误差 < 50ms */ }
TEST(FileSourceTest, PauseResume)      { /* pause 后 read 返回相同帧 */ }
TEST(CameraSourceTest, EnumerateDevices) { /* 返回非负数量 */ }
```

---

## 7. 关键性能指标

| 指标 | 目标 |
|------|------|
| 摄像头首帧延迟 | < 300ms |
| 文件读帧耗时 | < 5ms / 帧（1080p H.264） |
| 屏幕捕获延迟 | < 33ms（30fps 目标） |
| 内存（单帧 1080p BGR） | 约 6 MB |
