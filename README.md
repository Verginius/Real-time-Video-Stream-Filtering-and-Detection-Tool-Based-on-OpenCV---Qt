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
- **计算机视觉**：OpenCV 4.5.5（MinGW x64 预编译版）  
- **深度学习推理**：OpenCV DNN / ONNX Runtime  
- **目标检测模型**：YOLOv8 (Ultralytics ONNX 导出)  
- **构建系统**：CMake 3.16+  
- **测试框架**：Google Test  
- **平台**：Windows 10/11

---

## 目录结构

```
project-root/
├── CMakeLists.txt
├── cmake/
│   └── toolchain-qt6-mingw64.cmake        # MinGW 工具链文件
├── docs/
│   ├── design/                            # 各模块详细设计文档（DD-*.md）
│   └── assets/                            # 架构图、类图等 Mermaid 图表
├── libs/                                  # 第三方预编译库（不纳入版本控制，需手动放置）
│   └── OpenCV-MinGW-Build-OpenCV-4.5.5-x64/
│       ├── OpenCVConfig.cmake
│       ├── include/
│       └── x64/mingw/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── VideoController.h/cpp          # 帧循环中枢，协调所有子模块
│   │   ├── VideoSource/                   # 视频输入模块
│   │   │   ├── VideoSource.h              #   抽象基类
│   │   │   ├── CameraSource.h/cpp         #   摄像头输入
│   │   │   ├── FileSource.h/cpp           #   本地视频文件
│   │   │   └── ScreenSource.h/cpp         #   屏幕区域捕获
│   │   ├── Filter/                        # 滤镜模块
│   │   │   ├── FilterBase.h               #   抽象基类
│   │   │   ├── FilterChain.h/cpp          #   滤镜链（顺序执行）
│   │   │   ├── GrayscaleFilter.h/cpp      #   灰度化
│   │   │   ├── GaussianFilter.h/cpp       #   高斯模糊
│   │   │   ├── CannyFilter.h/cpp          #   Canny 边缘检测
│   │   │   ├── ThresholdFilter.h/cpp      #   二值化
│   │   │   └── HistEqFilter.h/cpp         #   直方图均衡化 / CLAHE
│   │   ├── Detection/                     # 目标检测模块
│   │   │   ├── Detection.h                #   Detection 结构体 + DetectionList typedef
│   │   │   ├── DetectorBase.h             #   抽象基类
│   │   │   ├── LabelMap.h                 #   类别 ID ↔ 名称 / 颜色映射
│   │   │   ├── YOLODetector.h/cpp         #   YOLOv8 ONNX 推理实现
│   │   │   └── DetectionRenderer.h/cpp    #   检测框可视化
│   │   └── Export/                        # 录制与导出模块
│   │       ├── VideoRecorder.h/cpp        #   视频录制（独立 I/O 线程）
│   │       └── ResultExporter.h/cpp       #   截图 + CSV/JSON 检测结果导出
│   └── ui/
│       ├── mainwindow.h/cpp/ui            # 主窗口
│       ├── FilterPanel.h/cpp              # 左侧滤镜面板
│       ├── DetectionPanel.h/cpp           # 右侧检测面板
│       └── VideoDisplay.h/cpp             # cv::Mat → QLabel 渲染工具
├── resources/                             # （规划中）
│   ├── models/                            #   ONNX 模型文件（不纳入版本控制）
│   ├── labels/                            #   COCO 类别标签
│   └── icons/
└── tests/                                 # （规划中）
    ├── test_filters.cpp
    ├── test_detector.cpp
    └── test_video_source.cpp
```

---

## 快速开始

### 前置依赖

**Windows（Qt 官方安装器 + MinGW）**

| 依赖 | 版本 | 获取方式 |
|------|------|---------|
| Qt | 6.10.2 mingw_64 | [Qt 官方在线安装器](https://www.qt.io/download) |
| MinGW | 13.1.0 64-bit | 随 Qt 安装器附带（`Tools/mingw1310_64`） |
| OpenCV | 4.5.5 MinGW x64 预编译 | [OpenCV-MinGW-Build](https://github.com/huihut/OpenCV-MinGW-Build) 下载并解压到 `libs/` |
| CMake | 3.16+ | [cmake.org](https://cmake.org/download/) 或随 Qt 安装 |

> **重要**：`libs/` 目录已加入 `.gitignore`，克隆仓库后需手动将 OpenCV 预编译包放置到以下路径：
> ```
> libs/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/
> ```
> CMake 将通过 `OpenCV_DIR` 自动找到该目录下的 `OpenCVConfig.cmake`。

### 构建

```bash
git clone <repo-url>
cd <project-dir>

# 手动放置 OpenCV 预编译包（见上方说明）

# Windows（使用项目自带工具链文件）
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-qt6-mingw64.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

### 运行

```bash
# Windows
build\RVSFDT.exe
```

> 运行前确保 OpenCV 的 `bin/` 目录（`libs/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/bin/`）已加入系统 `PATH`，或将对应 DLL 复制到可执行文件同级目录。

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

## 新增第三方库配置方法

以下以添加一个名为 `MyLib` 的预编译库为例，说明完整流程。

### 第一步：放置库文件

将预编译包解压后放入 `libs/` 目录，保持如下结构：

```
libs/
└── MyLib-1.0.0-x64/
    ├── MyLibConfig.cmake        # 若有官方 CMake 配置文件（可选）
    ├── include/
    │   └── mylib/
    │       └── mylib.h
    └── x64/mingw/
        ├── lib/
        │   └── libmylib.a       # 静态库或导入库
        └── bin/
            └── mylib.dll        # 动态库（运行时需要）
```

> `libs/` 已加入 `.gitignore`，**不会**随仓库提交，团队成员需各自手动放置。

---

### 第二步：修改 CMakeLists.txt

打开根目录的 `CMakeLists.txt`，参照以下两种方式之一进行配置：

**方式 A：库提供了官方 CMake 配置文件（`*Config.cmake`）**

```cmake
# 告知 CMake 到哪里查找 MyLibConfig.cmake
set(MyLib_DIR "${CMAKE_SOURCE_DIR}/libs/MyLib-1.0.0-x64")

find_package(MyLib REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE MyLib::MyLib)
```

**方式 B：手动指定头文件与库文件路径**

```cmake
set(MYLIB_ROOT "${CMAKE_SOURCE_DIR}/libs/MyLib-1.0.0-x64")

target_include_directories(${PROJECT_NAME} PRIVATE
    "${MYLIB_ROOT}/include"
)

target_link_directories(${PROJECT_NAME} PRIVATE
    "${MYLIB_ROOT}/x64/mingw/lib"
)

target_link_libraries(${PROJECT_NAME} PRIVATE mylib)
```

---

### 第三步：更新 VS Code IntelliSense 配置

为让 IntelliSense 正确识别新库的头文件，打开 `.vscode/c_cpp_properties.json`，在 `includePath` 数组中追加库的 `include` 路径：

```json
"includePath": [
    "${default}",
    "${workspaceFolder}/src",
    "${workspaceFolder}/libs/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/include",
    "${workspaceFolder}/libs/MyLib-1.0.0-x64/include",
    ...
]
```

保存后 IntelliSense 会自动重新索引，头文件中的 `#include` 红色报错即会消失。

---

### 第四步：确保 DLL 可在运行时找到

动态库（`.dll`）需在运行时可被系统找到，二选一：

- **方式 A（推荐开发期使用）**：将 `libs/MyLib-1.0.0-x64/x64/mingw/bin/` 加入系统 `PATH`  
- **方式 B（分发时使用）**：在 `CMakeLists.txt` 中通过 `install()` 或构建后脚本将 DLL 复制到可执行文件同级目录：

```cmake
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MYLIB_ROOT}/x64/mingw/bin/mylib.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
```

---

### 第五步：重新 CMake Configure 并构建

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-qt6-mingw64.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

在 VS Code 中也可通过状态栏执行 **CMake: Configure** → **CMake: Build**。

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
