# 图像滤镜模块详细设计

> **文档编号：** DD-FLT
> **所属阶段：** 第三阶段（第 3 周）
> **关联需求：** FLT-01 ~ FLT-10
> **状态：** 待实现

---

## 1. 模块职责

滤镜模块接收单帧 `cv::Mat`，按用户配置的滤镜链顺序依次处理，输出处理后的 `cv::Mat`。模块支持动态增删滤镜、参数热更新、以及完全关闭（直通）。

---

## 2. 类结构

```
FilterBase               (纯虚抽象基类)
├── GrayscaleFilter      FLT-01
├── GaussianFilter       FLT-02
├── CannyFilter          FLT-03
├── ThresholdFilter      FLT-04
├── HistEqFilter         FLT-05 (直方图均衡化 / CLAHE)
├── SharpenFilter        FLT-06
├── ColorSpaceFilter     FLT-07
├── MorphologyFilter     FLT-08
└── BgSubFilter          FLT-09

FilterChain              FLT-10（管理滤镜列表，顺序执行）
FilterParams             各滤镜参数 POD 结构体（通过 std::variant 统一传递）
```

---

## 3. 接口定义

### 3.1 FilterBase

```cpp
// src/core/Filter/FilterBase.h
#pragma once
#include <opencv2/core.hpp>
#include <string>

class FilterBase {
public:
    virtual ~FilterBase() = default;

    // 对 src 施加滤镜，返回处理后的帧（可与 src 共享内存或新分配）
    virtual cv::Mat apply(const cv::Mat& src) = 0;

    // 滤镜唯一标识符（用于序列化/UI 匹配）
    virtual std::string id() const = 0;

    // 人类可读名称
    virtual std::string name() const = 0;

    // 是否启用（禁用时 apply 直接返回 src 的克隆）
    bool enabled() const { return m_enabled; }
    void setEnabled(bool e) { m_enabled = e; }

protected:
    bool m_enabled = true;
};
```

---

### 3.2 FilterChain

```cpp
// src/core/Filter/FilterChain.h
#pragma once
#include "FilterBase.h"
#include <vector>
#include <memory>
#include <mutex>

class FilterChain {
public:
    using FilterPtr = std::shared_ptr<FilterBase>;

    // 追加滤镜到链末尾
    void append(FilterPtr filter);

    // 按 id 移除
    void remove(const std::string& filterId);

    // 调整顺序（移动索引 from → to）
    void move(std::size_t from, std::size_t to);

    // 清空
    void clear();

    // 顺序执行所有启用的滤镜（线程安全）
    cv::Mat process(const cv::Mat& src);

    // 获取指定 id 的滤镜（用于参数更新）
    FilterPtr find(const std::string& filterId);

    std::size_t size() const;

private:
    std::vector<FilterPtr> m_filters;
    mutable std::mutex     m_mutex;
};
```

**线程安全说明：** `process()` 与 `append/remove` 均持有 `m_mutex`，允许 UI 线程修改参数同时工作线程处理帧。

---

### 3.3 各滤镜实现

#### GrayscaleFilter（FLT-01）

```cpp
class GrayscaleFilter : public FilterBase {
public:
    std::string id()   const override { return "grayscale"; }
    std::string name() const override { return "灰度化"; }
    cv::Mat apply(const cv::Mat& src) override;
    // apply: cvtColor(BGR→GRAY)，输出再转回 BGR（保持通道数一致）
};
```

#### GaussianFilter（FLT-02）

```cpp
struct GaussianParams {
    int    kernelSize = 5;   // 奇数，范围 [1, 31]
    double sigmaX     = 1.0; // 0 = 自动
    double sigmaY     = 0.0; // 0 = 同 sigmaX
};

class GaussianFilter : public FilterBase {
public:
    explicit GaussianFilter(GaussianParams p = {});
    void   setParams(GaussianParams p);
    GaussianParams params() const;

    std::string id()   const override { return "gaussian"; }
    std::string name() const override { return "高斯模糊"; }
    cv::Mat apply(const cv::Mat& src) override;
    // apply: cv::GaussianBlur(src, dst, Size(k,k), sigmaX, sigmaY)
    // kernelSize 强制奇数化：if (k%2==0) k++

private:
    GaussianParams m_params;
    mutable std::mutex m_mu;
};
```

#### CannyFilter（FLT-03）

```cpp
struct CannyParams {
    double threshold1 = 50.0;
    double threshold2 = 150.0;
    int    apertureSize = 3;   // Sobel 核，3/5/7
    bool   l2gradient   = false;
};

class CannyFilter : public FilterBase {
public:
    explicit CannyFilter(CannyParams p = {});
    void setParams(CannyParams p);

    std::string id()   const override { return "canny"; }
    std::string name() const override { return "Canny 边缘检测"; }
    cv::Mat apply(const cv::Mat& src) override;
    // apply: 先灰度化 → cv::Canny → cvtColor(GRAY→BGR)
};
```

#### ThresholdFilter（FLT-04）

```cpp
enum class ThresholdType { Fixed, Adaptive, Otsu };

struct ThresholdParams {
    ThresholdType type      = ThresholdType::Fixed;
    double        value     = 127.0;      // Fixed 阈值
    int           blockSize = 11;         // Adaptive 邻域大小
    double        C         = 2.0;        // Adaptive 常数
};

class ThresholdFilter : public FilterBase {
public:
    explicit ThresholdFilter(ThresholdParams p = {});
    void setParams(ThresholdParams p);

    std::string id()   const override { return "threshold"; }
    std::string name() const override { return "二值化"; }
    cv::Mat apply(const cv::Mat& src) override;
    /*
     * Fixed:    cv::threshold(gray, dst, value, 255, THRESH_BINARY)
     * Adaptive: cv::adaptiveThreshold(gray, dst, 255,
     *               ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, blockSize, C)
     * Otsu:     cv::threshold(gray, dst, 0, 255, THRESH_BINARY | THRESH_OTSU)
     */
};
```

#### HistEqFilter（FLT-05）

```cpp
struct HistEqParams {
    bool   useCLAHE    = true;
    double clipLimit   = 2.0;
    int    tileGridW   = 8;
    int    tileGridH   = 8;
};

class HistEqFilter : public FilterBase {
public:
    explicit HistEqFilter(HistEqParams p = {});
    void setParams(HistEqParams p);

    std::string id()   const override { return "histeq"; }
    std::string name() const override { return "CLAHE 均衡化"; }
    cv::Mat apply(const cv::Mat& src) override;
    /*
     * 彩色图：转 YCrCb → 对 Y 通道做均衡化 → 转回 BGR
     * CLAHE: cv::createCLAHE(clipLimit, Size(tileW,tileH))->apply(Y, Y)
     * HistEq: cv::equalizeHist(Y, Y)
     */
};
```

#### SharpenFilter（FLT-06）

```cpp
struct SharpenParams {
    double strength = 1.0;   // Unsharp Mask 强度 [0.0, 3.0]
    double sigma    = 1.0;   // 模糊 sigma
};

class SharpenFilter : public FilterBase {
public:
    explicit SharpenFilter(SharpenParams p = {});
    void setParams(SharpenParams p);

    std::string id()   const override { return "sharpen"; }
    std::string name() const override { return "锐化"; }
    cv::Mat apply(const cv::Mat& src) override;
    /*
     * Unsharp Mask 公式：
     *   blurred = GaussianBlur(src, sigma)
     *   dst = src + strength * (src - blurred)
     *   cv::addWeighted(src, 1+strength, blurred, -strength, 0, dst)
     */
};
```

#### ColorSpaceFilter（FLT-07）

```cpp
enum class ColorSpaceTarget { BGR, HSV, YCrCb, Lab };

class ColorSpaceFilter : public FilterBase {
public:
    explicit ColorSpaceFilter(ColorSpaceTarget target = ColorSpaceTarget::HSV);
    void setTarget(ColorSpaceTarget t);

    std::string id()   const override { return "colorspace"; }
    std::string name() const override { return "色彩空间转换"; }
    cv::Mat apply(const cv::Mat& src) override;
    // apply: cvtColor(BGR→target)，显示时再转回 BGR（可视化）
};
```

#### MorphologyFilter（FLT-08）

```cpp
enum class MorphOp { Erode, Dilate, Open, Close, Gradient, TopHat, BlackHat };

struct MorphParams {
    MorphOp op          = MorphOp::Dilate;
    int     kernelSize  = 3;    // 奇数
    int     iterations  = 1;
};

class MorphologyFilter : public FilterBase {
public:
    explicit MorphologyFilter(MorphParams p = {});
    void setParams(MorphParams p);

    std::string id()   const override { return "morphology"; }
    std::string name() const override { return "形态学操作"; }
    cv::Mat apply(const cv::Mat& src) override;
};
```

#### BgSubFilter（FLT-09）

```cpp
enum class BgSubAlgo { MOG2, KNN };

struct BgSubParams {
    BgSubAlgo algo           = BgSubAlgo::MOG2;
    int       history        = 500;
    double    threshold      = 16.0;  // MOG2 varThreshold / KNN dist2Threshold
    bool      detectShadows  = false;
};

class BgSubFilter : public FilterBase {
public:
    explicit BgSubFilter(BgSubParams p = {});
    void setParams(BgSubParams p);
    void resetBackground();   // 清空背景模型

    std::string id()   const override { return "bgsub"; }
    std::string name() const override { return "背景差分"; }
    cv::Mat apply(const cv::Mat& src) override;
    // apply: pBackSub->apply(src, fgMask) → cvtColor(GRAY→BGR)
    // 参数变更时重新 create 子分离器

private:
    BgSubParams m_params;
    cv::Ptr<cv::BackgroundSubtractor> m_pBS;
    mutable std::mutex m_mu;
};
```

---

## 4. 参数热更新机制

所有 `setParams()` 方法内部持有 `std::mutex`，保证工作线程正在调用 `apply()` 时 UI 线程写入参数是安全的。UI 控件（滑块/下拉框）通过 Qt 信号槽调用对应 `setParams()`，无需重建滤镜对象。

---

## 5. 滤镜链执行流程

```
                ┌──────────────────────────────────┐
原始帧 ──────► │  FilterChain::process(src)        │
                │  for each filter (enabled only):  │
                │    frame = filter->apply(frame)   │
                └──────────────────────────────────┘
                                │
                          处理后帧 ──► VideoController
```

---

## 6. 线程安全矩阵

| 操作 | 调用线程 | 保护方式 |
|------|---------|---------|
| `FilterChain::process()` | 工作线程 | `m_mutex` |
| `FilterChain::append/remove` | UI 线程 | `m_mutex` |
| `XxxFilter::setParams()` | UI 线程 | 各滤镜内部 mutex |
| `XxxFilter::apply()` | 工作线程 | 各滤镜内部 mutex |

---

## 7. 单元测试要点（test_filters.cpp）

```cpp
TEST(GrayscaleFilterTest, OutputChannels)   { /* 输出 3 通道 BGR */ }
TEST(GaussianFilterTest, EvenKernelForced)  { /* kernelSize=4 → 实际使用 5 */ }
TEST(CannyFilterTest, OutputIsBinary)        { /* 输出仅含 0 和 255 */ }
TEST(ThresholdFilterTest, OtsuAutoThresh)   { /* Otsu 模式阈值自动确定 */ }
TEST(HistEqFilterTest, MeanBrightnessUp)    { /* 均值亮度提升 */ }
TEST(FilterChainTest, DisabledFilterSkip)   { /* 禁用后 apply 直通 */ }
TEST(FilterChainTest, OrderMatters)          { /* 灰度→Canny vs Canny→灰度 结果不同 */ }
TEST(BgSubFilterTest, ResetBackground)      { /* reset 后前景全白 */ }
```

---

## 8. 关键性能指标

| 滤镜 | 分辨率 | 目标耗时 |
|------|--------|---------|
| 灰度化 | 1080p | < 2 ms |
| 高斯模糊（k=5） | 1080p | < 5 ms |
| Canny | 1080p | < 8 ms |
| 二值化 | 1080p | < 3 ms |
| CLAHE | 1080p | < 10 ms |
| 背景差分（MOG2） | 1080p | < 15 ms |
| 完整滤镜链（所有 P0/P1）| 1080p | < 30 ms |
