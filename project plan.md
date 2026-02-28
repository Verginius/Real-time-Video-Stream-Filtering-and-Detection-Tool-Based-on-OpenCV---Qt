# 基于 OpenCV + Qt 的实时视频流滤镜与检测工具 — 项目计划

> **项目名称：** Real-time Video Stream Filtering and Detection Tool Based on OpenCV + Qt
> **文档版本：** v1.0
> **创建日期：** 2026-02-28
> **项目规模：** 小型（个人 / 小团队）
> **预期周期：** 6 ~ 8 周

---

## 1. 项目概述

### 1.1 目标

构建一个跨平台的桌面应用程序，允许用户从多种来源（本地摄像头、本地视频文件、屏幕录制流）读取视频，实时施加图像滤镜处理，并利用 YOLO 系列模型对视频帧进行目标检测，最终以直观的 Qt GUI 呈现结果并支持录制导出。

### 1.2 核心价值

- **实时性**：视频处理与检测延迟 < 100ms（1080p @ 30fps 目标）
- **易用性**：GUI 操作无需命令行，参数可视化调节
- **可扩展性**：滤镜与检测模块插件化，便于后续添加新算法
- **通用性**：支持多种输入源与多种导出格式

---

## 2. 技术栈

| 层级 | 技术选型 | 版本要求 |
|------|----------|----------|
| UI 框架 | Qt | 6.x（推荐） / 5.15+ |
| 计算机视觉 | OpenCV | 4.8+ |
| 深度学习推理 | OpenCV DNN / ONNX Runtime | — |
| 目标检测模型 | YOLOv8 (Ultralytics ONNX 导出) | — |
| 编程语言 | C++17 | — |
| 构建系统 | CMake | 3.20+ |
| 版本控制 | Git | — |
| 测试框架 | Google Test (gtest) | — |
| 平台支持 | Windows 10/11, Ubuntu 22.04, macOS 12+ | — |

---

## 3. 功能需求

### 3.1 视频输入模块

| ID | 功能 | 优先级 |
|----|------|--------|
| VIN-01 | 打开本地摄像头（USB / 内置），支持多摄像头选择 | P0 |
| VIN-02 | 打开本地视频文件（MP4, AVI, MKV, MOV） | P0 |
| VIN-03 | 屏幕区域录制流（全屏 / 指定窗口 / 自定义区域） | P1 |
| VIN-04 | 显示当前帧率（FPS）与分辨率信息 | P1 |
| VIN-05 | 支持暂停 / 继续 / 逐帧步进 | P1 |

### 3.2 图像滤镜模块

| ID | 功能 | 优先级 |
|----|------|--------|
| FLT-01 | 灰度化 | P0 |
| FLT-02 | 高斯模糊（可调 kernel 大小与 sigma） | P0 |
| FLT-03 | 边缘检测（Canny，可调阈值） | P0 |
| FLT-04 | 二值化（固定阈值 / 自适应 / Otsu） | P0 |
| FLT-05 | 直方图均衡化 / CLAHE | P1 |
| FLT-06 | 锐化（Laplacian / Unsharp Mask） | P1 |
| FLT-07 | 色彩空间转换（RGB / HSV / YCrCb） | P1 |
| FLT-08 | 膨胀 / 腐蚀 / 形态学操作 | P2 |
| FLT-09 | 背景差分（MOG2 / KNN 运动检测） | P2 |
| FLT-10 | 滤镜链（多个滤镜按序叠加） | P1 |

### 3.3 目标检测模块

| ID | 功能 | 优先级 |
|----|------|--------|
| DET-01 | 加载 YOLOv8 ONNX 模型 | P0 |
| DET-02 | 实时目标检测并在帧上绘制 Bounding Box + 类别标签 + 置信度 | P0 |
| DET-03 | 置信度阈值 / NMS 阈值可调 | P0 |
| DET-04 | 支持 COCO 80 类标签，也支持自定义类别文件 | P1 |
| DET-05 | 检测帧率独立于显示帧率（可跳帧推理以保证流畅） | P1 |
| DET-06 | GPU 加速支持（CUDA / OpenCL 后端可选） | P2 |

### 3.4 GUI 模块

| ID | 功能 | 优先级 |
|----|------|--------|
| GUI-01 | 主窗口：原始帧 / 处理后帧 双视图布局 | P0 |
| GUI-02 | 侧边栏：滤镜开关面板，参数滑块 | P0 |
| GUI-03 | 工具栏：打开输入源、开始/停止、录制 | P0 |
| GUI-04 | 状态栏：FPS、分辨率、检测数量 | P1 |
| GUI-05 | 检测结果列表（类别、置信度、位置） | P1 |
| GUI-06 | 截图保存当前帧 | P1 |
| GUI-07 | 深色 / 浅色主题切换 | P2 |

### 3.5 录制与导出模块

| ID | 功能 | 优先级 |
|----|------|--------|
| EXP-01 | 录制处理后的视频（MP4 / AVI） | P1 |
| EXP-02 | 导出检测结果为 CSV / JSON | P2 |
| EXP-03 | 截图保存为 PNG / JPEG | P1 |

---

## 4. 系统架构

```
┌─────────────────────────────────────────────────────────┐
│                        Qt GUI Layer                      │
│  MainWindow  │  FilterPanel  │  DetectionPanel  │  ...  │
└────────────────────────┬────────────────────────────────┘
                         │ signals / slots
┌────────────────────────▼────────────────────────────────┐
│                   VideoController                        │
│  (协调输入、处理管道、检测器、输出之间的数据流)              │
└───┬─────────────────────────────────┬───────────────────┘
    │                                 │
┌───▼───────────┐           ┌─────────▼──────────┐
│  VideoSource  │           │  ProcessingPipeline │
│  - CameraSource│          │  - FilterChain      │
│  - FileSource │           │  - FilterBase       │
│  - ScreenSource│          │  - GaussianFilter   │
└───────────────┘           │  - CannyFilter      │
                            │  - ... (可扩展)      │
                            └─────────┬────────────┘
                                      │
                            ┌─────────▼────────────┐
                            │    YOLODetector       │
                            │  (OpenCV DNN / ONNX)  │
                            └─────────┬────────────┘
                                      │
                            ┌─────────▼────────────┐
                            │    VideoWriter        │
                            │  (录制 / 截图导出)    │
                            └──────────────────────┘
```

### 4.1 核心类设计

```cpp
// 视频输入抽象接口
class VideoSource {
public:
    virtual bool open() = 0;
    virtual bool read(cv::Mat& frame) = 0;
    virtual void close() = 0;
    virtual ~VideoSource() = default;
};

// 滤镜抽象接口
class FilterBase {
public:
    virtual cv::Mat apply(const cv::Mat& src) = 0;
    virtual std::string name() const = 0;
    virtual ~FilterBase() = default;
};

// 检测器抽象接口
class DetectorBase {
public:
    virtual std::vector<Detection> detect(const cv::Mat& frame) = 0;
    virtual ~DetectorBase() = default;
};

struct Detection {
    cv::Rect bbox;
    int classId;
    float confidence;
    std::string label;
};
```

---

## 5. 目录结构

```
project-root/
├── CMakeLists.txt
├── project plan.md
├── README.md
├── docs/
│   ├── plans/
│   └── assets/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── VideoController.h/cpp
│   │   ├── VideoSource/
│   │   │   ├── VideoSourceBase.h
│   │   │   ├── CameraSource.h/cpp
│   │   │   ├── FileSource.h/cpp
│   │   │   └── ScreenSource.h/cpp
│   │   ├── Filter/
│   │   │   ├── FilterBase.h
│   │   │   ├── FilterChain.h/cpp
│   │   │   ├── GaussianFilter.h/cpp
│   │   │   ├── CannyFilter.h/cpp
│   │   │   ├── GrayscaleFilter.h/cpp
│   │   │   ├── ThresholdFilter.h/cpp
│   │   │   └── HistEqFilter.h/cpp
│   │   ├── Detection/
│   │   │   ├── DetectorBase.h
│   │   │   ├── YOLODetector.h/cpp
│   │   │   └── Detection.h
│   │   └── Export/
│   │       ├── VideoWriter.h/cpp
│   │       └── ResultExporter.h/cpp
│   └── ui/
│       ├── MainWindow.h/cpp
│       ├── MainWindow.ui
│       ├── FilterPanel.h/cpp
│       ├── DetectionPanel.h/cpp
│       └── VideoDisplay.h/cpp
├── resources/
│   ├── models/          # ONNX 模型文件
│   ├── labels/          # COCO 类别标签
│   └── icons/
└── tests/
    ├── CMakeLists.txt
    ├── test_filters.cpp
    ├── test_detector.cpp
    └── test_video_source.cpp
```

---

## 6. 开发阶段规划

### 第一阶段：环境搭建与骨架（第 1 周）

**目标：** 项目可以编译运行，显示空白 Qt 主窗口

- [ ] 初始化 Git 仓库，建立目录结构
- [ ] 配置 CMakeLists.txt（Qt6 + OpenCV + gtest）
- [ ] 创建 MainWindow 基础布局（双视图 + 工具栏 + 状态栏）
- [ ] 实现 VideoDisplay 控件（QLabel 或 QOpenGLWidget 显示 cv::Mat）
- [ ] 搭建 VideoController 骨架，建立处理线程（QThread）

**验收标准：** 程序编译无错误，主窗口正常显示

---

### 第二阶段：视频输入模块（第 2 周）

**目标：** 支持打开摄像头与本地视频文件并实时显示

- [ ] 实现 `CameraSource`（枚举设备、cv::VideoCapture）
- [ ] 实现 `FileSource`（打开 MP4/AVI，支持暂停/继续/逐帧）
- [ ] 实现 `ScreenSource`（平台相关屏幕捕获）
- [ ] 在工具栏添加"打开输入源"对话框
- [ ] 状态栏显示实时 FPS 与分辨率
- [ ] 编写单元测试（test_video_source.cpp）

**验收标准：** 能流畅播放 1080p@30fps 视频，FPS 显示准确

---

### 第三阶段：图像滤镜模块（第 3 周）

**目标：** 实现 P0/P1 级别滤镜，支持实时参数调节

- [ ] 实现 FilterBase 接口与 FilterChain
- [ ] 实现 GrayscaleFilter、GaussianFilter、CannyFilter、ThresholdFilter
- [ ] 实现 HistEqFilter（直方图均衡化 / CLAHE）
- [ ] 实现锐化滤镜、色彩空间转换滤镜
- [ ] 构建 FilterPanel UI（开关 + 滑块，信号绑定 FilterChain）
- [ ] 编写单元测试（test_filters.cpp）

**验收标准：** 所有 P0/P1 滤镜实时生效，参数拖动无卡顿

---

### 第四阶段：目标检测模块（第 4 ~ 5 周）

**目标：** 集成 YOLOv8 ONNX 模型，实现实时检测

- [ ] 下载 / 转换 YOLOv8n ONNX 模型
- [ ] 实现 YOLODetector（预处理 → DNN推理 → NMS后处理 → Detection 列表）
- [ ] 在帧上绘制 BBox、标签、置信度
- [ ] 实现检测跳帧策略（每 N 帧推理一次，保证主线程流畅）
- [ ] DetectionPanel UI（置信度/NMS 阈值调节，检测结果列表）
- [ ] 支持加载自定义类别文件
- [ ] 编写单元测试（test_detector.cpp）

**验收标准：** COCO 数据集目标可正确检测，mAP 与官方接近；检测延迟 < 50ms（CPU）

---

### 第五阶段：导出与录制（第 6 周）

**目标：** 支持截图与处理后视频录制

- [ ] 实现截图功能（PNG/JPEG 文件名含时间戳）
- [ ] 实现 VideoWriter（录制处理后帧 + 检测叠加层）
- [ ] 工具栏"录制"按钮，录制状态指示
- [ ] （可选）检测结果导出为 CSV / JSON

**验收标准：** 录制视频可正常播放，帧率与原视频一致

---

### 第六阶段：优化与测试（第 7 ~ 8 周）

**目标：** 性能调优、UI 完善、跨平台测试

- [ ] 性能剖析，找出瓶颈（优先优化推理线程）
- [ ] GPU 加速后端（CUDA/OpenCL）可选开关
- [ ] 完善所有单元测试，覆盖率 > 70%
- [ ] 跨平台编译验证（Windows / Linux）
- [ ] 用户文档（README + 使用说明）
- [ ] 代码审查与重构

**验收标准：** 1080p@30fps 全流程（输入→滤镜→检测→显示）CPU 占用 < 60%

---

## 7. 性能目标

| 指标 | 目标值 |
|------|--------|
| 端到端延迟（输入到显示） | < 100ms |
| 视频显示帧率 | 30 FPS（1080p） |
| YOLO 推理帧率（CPU, YOLOv8n） | ≥ 10 FPS |
| YOLO 推理帧率（GPU, YOLOv8n） | ≥ 30 FPS |
| 内存占用 | < 512 MB（不含模型） |

---

## 8. 风险与对策

| 风险 | 概率 | 影响 | 对策 |
|------|------|------|------|
| Windows 屏幕捕获 API 兼容性 | 中 | 中 | 优先使用 DXGI，备选 BitBlt |
| ONNX Runtime 与 OpenCV DNN 性能差异 | 低 | 低 | 封装统一接口，后期可切换 |
| Qt6 与 OpenCV Mat 显示性能 | 低 | 高 | 使用 QImage::fromData 零拷贝方式 |
| YOLO 推理 CPU 负载过高 | 中 | 高 | 实现跳帧推理 + 独立线程 |
| 多线程竞争（帧队列） | 中 | 高 | 使用有界线程安全队列（环形缓冲） |

---

## 9. 开发规范

### 代码规范
- 遵循 Google C++ Style Guide
- 类名 `PascalCase`，函数名 `camelCase`，成员变量 `m_` 前缀
- 每个 `.cpp` / `.h` 文件头部包含版权注释与文件描述

### Git 规范
- 主分支：`main`（只合并经测试的代码）
- 功能分支：`feature/<功能名>`
- 修复分支：`fix/<问题描述>`
- Commit message 格式：`<type>: <subject>`（feat / fix / refactor / test / docs / chore）

### 测试规范
- 每个核心模块均需对应单元测试
- 新功能合并前测试全部通过
- 使用 Google Test，测试文件与源文件一一对应

---

## 10. 依赖安装指南

### Windows（vcpkg）
```bash
vcpkg install opencv4[contrib,dnn]:x64-windows
vcpkg install qt6:x64-windows
vcpkg install gtest:x64-windows
```

### Ubuntu 22.04
```bash
sudo apt install -y libopencv-dev qt6-base-dev libgtest-dev cmake ninja-build
```

### macOS（Homebrew）
```bash
brew install opencv qt@6 googletest cmake
```

---

## 11. 快速启动

```bash
git clone <repo-url>
cd <project-dir>
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
./VideoFilterTool
```

---

## 12. 里程碑一览

| 里程碑 | 目标 | 预计完成 |
|--------|------|----------|
| M0 | 环境搭建，主窗口可运行 | 第 1 周末 |
| M1 | 视频输入模块完成 | 第 2 周末 |
| M2 | 滤镜模块完成 | 第 3 周末 |
| M3 | YOLO 检测模块集成完成 | 第 5 周末 |
| M4 | 录制与导出完成 | 第 6 周末 |
| M5 | 性能优化与测试完成，v1.0 发布 | 第 8 周末 |

---

*文档维护：每完成一个里程碑后更新进度，若需求变更需同步修订本文档。*
