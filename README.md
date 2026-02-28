# Real-time Video Stream Filtering and Detection Tool

基于 **OpenCV + Qt 6** 的实时视频流滤镜与目标检测桌面工具。

---

## 功能概览

| 模块 | 主要功能 |
|------|---------|
| 视频输入 | 本地摄像头、视频文件（MP4/AVI/MKV）、屏幕录制流 |
| 图像滤镜 | 灰度化、高斯模糊、Canny 边缘、二值化、CLAHE、锐化、形态学、背景差分等，支持滤镜链叠加 |
| 目标检测 | YOLOv8 ONNX 实时推理，可视化 Bounding Box + 类别 + 置信度 |
| 导出 | 截图（PNG/JPEG）、处理后视频录制（MP4/AVI）、检测结果 CSV/JSON |

---

## 技术栈

- **语言**：C++17  
- **UI 框架**：Qt 6.10.2  
- **计算机视觉**：OpenCV 4.8+  
- **深度学习推理**：OpenCV DNN / ONNX Runtime  
- **目标检测模型**：YOLOv8 (Ultralytics ONNX 导出)  
- **构建系统**：CMake 3.20+  
- **测试框架**：Google Test  
- **平台**：Windows 10/11、Ubuntu 22.04

---

## 目录结构

```
project-root/
├── CMakeLists.txt
├── cmake/
│   └── toolchain-qt6-mingw64.cmake   # MinGW 工具链文件
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── VideoController.h/cpp
│   │   ├── VideoSource/              # 摄像头 / 文件 / 屏幕输入
│   │   ├── Filter/                   # 滤镜链与各滤镜实现
│   │   ├── Detection/                # YOLODetector
│   │   └── Export/                   # 录制与导出
│   └── ui/
│       ├── mainwindow.h/cpp/ui
│       ├── FilterPanel.h/cpp
│       ├── DetectionPanel.h/cpp
│       └── VideoDisplay.h/cpp
├── resources/
│   ├── models/                       # ONNX 模型文件（不纳入版本控制）
│   ├── labels/                       # COCO 类别标签
│   └── icons/
└── tests/
    ├── test_filters.cpp
    ├── test_detector.cpp
    └── test_video_source.cpp
```

---

## 快速开始

### 前置依赖

**Windows（Qt 官方安装器 + MinGW）**

| 依赖 | 推荐版本 | 安装方式 |
|------|---------|---------|
| Qt | 6.10.2 mingw_64 | [Qt 官方在线安装器](https://www.qt.io/download) |
| MinGW | 13.1.0 64-bit | 随 Qt 安装器附带 (`Tools/mingw1310_64`) |
| OpenCV | 4.8+ | [opencv.org](https://opencv.org/releases/) 或 vcpkg |
| CMake | 3.20+ | [cmake.org](https://cmake.org/download/) 或随 Qt 安装 |

**Ubuntu 22.04**

```bash
sudo apt install -y libopencv-dev qt6-base-dev cmake ninja-build libgtest-dev
```

### 构建

```bash
git clone <repo-url>
cd <project-dir>

# Windows（使用项目自带工具链文件）
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-qt6-mingw64.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

# Linux / macOS
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

### 运行

```bash
# Windows
build\RVSFDT.exe

# Linux
./build/RVSFDT
```

---

## VS Code 开发环境配置（Windows）

项目已在 `.vscode/` 中预置以下配置：

| 文件 | 说明 |
|------|------|
| `cmake-kits.json` | CMake Kit：Qt 6.10.2 MinGW 13.1.0 64-bit |
| `settings.json` | 指定默认 Kit 与工具链文件 |
| `tasks.json` | `cmake --build build` 构建任务 |
| `launch.json` | GDB 调试配置（`RVSFDT.exe`） |

在状态栏选择 Kit **Qt 6.10.2 MinGW 13.1.0 64-bit** → 执行 **CMake: Configure** → **CMake: Build** 即可。

---

## 性能目标

| 指标 | 目标值 |
|------|--------|
| 端到端延迟（输入→显示） | < 100 ms |
| 视频显示帧率 | 30 FPS（1080p） |
| YOLO 推理帧率（CPU，YOLOv8n） | ≥ 10 FPS |
| YOLO 推理帧率（GPU，YOLOv8n） | ≥ 30 FPS |
| 内存占用 | < 512 MB（不含模型） |

---

## 开发进度

| 里程碑 | 内容 | 状态 |
|--------|------|------|
| M0 | 环境搭建，主窗口可运行 | 🔄 进行中 |
| M1 | 视频输入模块完成 | ⬜ 待开始 |
| M2 | 滤镜模块完成 | ⬜ 待开始 |
| M3 | YOLO 检测模块集成完成 | ⬜ 待开始 |
| M4 | 录制与导出完成 | ⬜ 待开始 |
| M5 | 性能优化与测试，v1.0 发布 | ⬜ 待开始 |

详细计划见 [project plan.md](project%20plan.md)。

---

## 贡献规范

- 分支命名：`feature/<功能名>` / `fix/<问题描述>`  
- Commit 格式：`feat:` / `fix:` / `refactor:` / `test:` / `docs:` / `chore:`  
- 合并前须通过所有单元测试

---

## License

MIT
