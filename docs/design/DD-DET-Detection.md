# 目标检测模块详细设计

> **文档编号：** DD-DET
> **所属阶段：** 第四阶段（第 4 ~ 5 周）
> **关联需求：** DET-01 ~ DET-06
> **状态：** 待实现

---

## 1. 模块职责

目标检测模块接收处理后的 `cv::Mat` 帧，使用 YOLOv8 ONNX 模型进行推理，输出 `Detection` 列表（含边界框、类别、置信度），并支持在帧上叠加可视化标注。

---

## 2. 类结构

```
DetectorBase             (纯虚抽象基类)
└── YOLODetector         基于 OpenCV DNN 的 YOLOv8 实现

Detection                结果 POD 结构体
DetectionRenderer        在帧上绘制检测结果的工具类
LabelMap                 类别 ID ↔ 名称映射（COCO / 自定义）
```

---

## 3. 数据结构

```cpp
// src/core/Detection/Detection.h
#pragma once
#include <opencv2/core.hpp>
#include <string>
#include <vector>

struct Detection {
    cv::Rect2f bbox;        // 像素坐标（浮点，保留子像素精度）
    int        classId;     // 类别 ID（COCO 0~79）
    float      confidence;  // 置信度 [0,1]
    std::string label;      // 类别名称字符串
};

using DetectionList = std::vector<Detection>;
```

---

## 4. 接口定义

### 4.1 DetectorBase

```cpp
// src/core/Detection/DetectorBase.h
#pragma once
#include "Detection.h"
#include <opencv2/core.hpp>
#include <string>

class DetectorBase {
public:
    virtual ~DetectorBase() = default;

    // 加载模型（路径、标签文件路径）
    virtual bool loadModel(const std::string& modelPath,
                           const std::string& labelsPath = "") = 0;

    // 对 frame 执行推理，返回检测结果列表
    virtual DetectionList detect(const cv::Mat& frame) = 0;

    // 模型是否已加载
    virtual bool isLoaded() const = 0;

    // 设置置信度阈值（[0,1]）
    virtual void setConfThreshold(float thresh) = 0;
    virtual float confThreshold() const = 0;

    // 设置 NMS IOU 阈值
    virtual void setNmsThreshold(float thresh) = 0;
    virtual float nmsThreshold() const = 0;
};
```

---

### 4.2 YOLODetector

```cpp
// src/core/Detection/YOLODetector.h
#pragma once
#include "DetectorBase.h"
#include "LabelMap.h"
#include <opencv2/dnn.hpp>
#include <mutex>

struct YOLOConfig {
    int    inputWidth   = 640;
    int    inputHeight  = 640;
    float  confThresh   = 0.50f;
    float  nmsThresh    = 0.45f;
    int    backendId    = cv::dnn::DNN_BACKEND_DEFAULT; // CUDA=cv::dnn::DNN_BACKEND_CUDA
    int    targetId     = cv::dnn::DNN_TARGET_CPU;      // CUDA=DNN_TARGET_CUDA
};

class YOLODetector : public DetectorBase {
public:
    explicit YOLODetector(YOLOConfig cfg = {});

    bool loadModel(const std::string& modelPath,
                   const std::string& labelsPath = "") override;
    DetectionList detect(const cv::Mat& frame)    override;
    bool  isLoaded()        const override;
    void  setConfThreshold(float t) override;
    float confThreshold()  const override;
    void  setNmsThreshold(float t) override;
    float nmsThreshold()   const override;

    // 设置后端（需重新加载模型）
    void setBackend(int backendId, int targetId);

    // 获取推理耗时（ms，最近一次）
    double lastInferenceMsec() const;

private:
    // --- 推理流程 ---
    cv::Mat   preprocess(const cv::Mat& frame);
    DetectionList postprocess(const std::vector<cv::Mat>& outputs,
                              const cv::Size& origSize);

    YOLOConfig      m_cfg;
    cv::dnn::Net    m_net;
    LabelMap        m_labels;
    bool            m_loaded = false;
    mutable std::mutex m_mutex;
    double          m_lastInfMs = 0.0;
    cv::Size        m_inputSize;
};
```

---

### 4.3 推理流程详解

#### 4.3.1 预处理（`preprocess`）

```
输入帧（任意尺寸 BGR）
    │
    ▼ letterbox 填充（保持纵横比）到 640×640
    │   padding = 灰色(114,114,114)
    ▼ BGR → RGB
    ▼ uint8 → float32，归一化 /255.0
    ▼ NHWC → NCHW  (blobFromImage)
    └─► cv::Mat blob [1, 3, 640, 640]
```

```cpp
cv::Mat YOLODetector::preprocess(const cv::Mat& frame) {
    cv::Mat letterboxed;
    // 1. Letterbox resize
    float scale = std::min(m_inputSize.width  / float(frame.cols),
                           m_inputSize.height / float(frame.rows));
    cv::resize(frame, letterboxed,
               cv::Size(int(frame.cols*scale), int(frame.rows*scale)),
               0, 0, cv::INTER_LINEAR);
    cv::Mat padded(m_inputSize, CV_8UC3, cv::Scalar(114,114,114));
    letterboxed.copyTo(padded(cv::Rect(0, 0,
                                       letterboxed.cols, letterboxed.rows)));
    // 2. Blob
    return cv::dnn::blobFromImage(padded, 1.0/255.0,
                                  m_inputSize, cv::Scalar(), true, false, CV_32F);
}
```

#### 4.3.2 后处理（`postprocess`）

YOLOv8 ONNX 输出形状：`[1, 84, 8400]`（COCO 80 类 + 4 坐标）

```
输出张量 [1, 84, 8400]
    │  转置 → [8400, 84]
    ▼
    for each row:
        cx, cy, w, h = row[0..3]   (归一化到 inputSize)
        scores = row[4..83]
        classId = argmax(scores)
        conf = scores[classId]
        if conf < confThresh: skip
        还原到原图坐标（考虑 letterbox 偏移与 scale）
        push_back Detection{bbox, classId, conf, labelName}
    ▼
    cv::dnn::NMSBoxes(boxes, scores, confThresh, nmsThresh, indices)
    ▼
    筛选 indices 对应的 Detection → 返回 DetectionList
```

---

### 4.4 LabelMap

```cpp
// src/core/Detection/LabelMap.h
#pragma once
#include <vector>
#include <string>

class LabelMap {
public:
    // 加载 txt（每行一个类别名称）或内置 COCO80
    bool loadFromFile(const std::string& path);
    void loadCOCO80();

    const std::string& nameOf(int classId) const;   // 越界返回 "unknown"
    int size() const;

    // 为每个类别生成固定颜色（BGR），用于可视化
    cv::Scalar colorOf(int classId) const;

private:
    std::vector<std::string>  m_names;
    mutable std::vector<cv::Scalar> m_colors;  // 惰性生成
};
```

---

### 4.5 DetectionRenderer

```cpp
// src/core/Detection/DetectionRenderer.h
#pragma once
#include "Detection.h"
#include "LabelMap.h"
#include <opencv2/imgproc.hpp>

class DetectionRenderer {
public:
    struct Style {
        int   boxThickness  = 2;
        float fontScale     = 0.55f;
        int   fontThickness = 1;
        bool  showScore     = true;
        bool  showLabel     = true;
    };

    explicit DetectionRenderer(const LabelMap& labels, Style style = {});

    // 在 frame 上原地绘制所有检测框（frame 须为可写副本）
    void render(cv::Mat& frame, const DetectionList& detections) const;

private:
    const LabelMap& m_labels;
    Style           m_style;
};
```

---

## 5. 跳帧推理策略

为保证 GUI 流畅度（30fps），检测推理（CPU 约 100ms/帧）在独立线程中运行，采用**跳帧策略**：

```
工作线程帧计数器 n
    │
    ├── n % skipInterval == 0 → 提交帧到检测线程（异步）
    └── 其余帧 → 复用上次检测结果叠加显示

std::atomic<DetectionList> m_latestDetections;  // 原子替换
```

```
┌─ VideoController 帧循环（30fps）─────────────────────────┐
│  frame = source->read()                                   │
│  processedFrame = filterChain.process(frame)             │
│  if (frameCount % skipN == 0)                            │
│      detectorThread.submit(processedFrame)               │  异步
│  renderer.render(processedFrame, latestDetections)       │
│  emit frameReady(processedFrame, latestDetections)       │
└──────────────────────────────────────────────────────────┘
           ↑                ↓
┌─ DetectorThread ──────────────────────────────────────────┐
│  frame = queue.dequeue()                                  │
│  results = yoloDetector.detect(frame)                    │
│  latestDetections.store(results)                         │
└──────────────────────────────────────────────────────────┘
```

---

## 6. GPU 加速（P2）

| 后端 | 设置方式 | 前提 |
|------|---------|-----|
| CUDA | `DNN_BACKEND_CUDA + DNN_TARGET_CUDA` | OpenCV with CUDA build, NVIDIA GPU |
| OpenCL | `DNN_BACKEND_DEFAULT + DNN_TARGET_OPENCL` | OpenCL 驱动 |
| CPU（默认）| `DNN_BACKEND_DEFAULT + DNN_TARGET_CPU` | 无 |

通过 `YOLODetector::setBackend()` 在运行时切换，切换后需重新调用 `loadModel()` 使后端生效。

---

## 7. 单元测试要点（test_detector.cpp）

```cpp
TEST(LabelMapTest, COCO80Count)          { /* 80 个类别 */ }
TEST(LabelMapTest, NameOfPerson)         { /* classId=0 → "person" */ }
TEST(YOLODetectorTest, LoadValidModel)   { /* isLoaded()=true */ }
TEST(YOLODetectorTest, DetectOnBlank)    { /* 全黑帧检测结果为空 */ }
TEST(YOLODetectorTest, ConfThreshFilter) { /* 调高阈值后检测数量减少 */ }
TEST(YOLODetectorTest, NMSDedup)         { /* 重叠框被 NMS 合并 */ }
```

---

## 8. 关键性能指标

| 指标 | CPU（YOLOv8n） | GPU（YOLOv8n, CUDA）|
|------|--------------|---------------------|
| 单帧推理耗时 | < 100 ms | < 15 ms |
| 推理帧率 | ≥ 10 FPS | ≥ 30 FPS |
| 模型内存占用 | ~12 MB | ~12 MB + GPU |
| 后处理耗时 | < 5 ms | < 5 ms |

---

## 9. 模型获取

```bash
# 使用 Ultralytics 导出 YOLOv8n ONNX
pip install ultralytics
python -c "from ultralytics import YOLO; YOLO('yolov8n.pt').export(format='onnx', imgsz=640, opset=12)"
# 输出：yolov8n.onnx  (~6 MB)
# 放置到：resources/models/yolov8n.onnx
```
