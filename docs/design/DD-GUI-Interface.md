# GUI 模块详细设计

> **文档编号：** DD-GUI
> **所属阶段：** 第一阶段（骨架）+ 各阶段持续完善
> **关联需求：** GUI-01 ~ GUI-07
> **状态：** 骨架已完成，待完善

---

## 1. 模块职责

GUI 模块负责整个桌面应用程序的界面呈现、用户交互和状态反馈。通过 Qt 信号槽机制与 VideoController 解耦：GUI 只发送意图信号，VideoController 响应并回传数据。

---

## 2. 窗口与控件结构

```
MainWindow  (QMainWindow)
├── QMenuBar
│   ├── 文件菜单 (actionOpenCamera / actionOpenFile / actionOpenScreen / actionExit)
│   ├── 视图菜单 (actionShowFilter / actionShowDetection)
│   └── 帮助菜单 (actionAbout)
├── QToolBar  (mainToolBar)
│   └── 打开摄像头 | 打开文件 | 屏幕捕获 | --- | 播放/暂停 | 停止 | --- | 截图 | 录制
├── centralwidget  [QHBoxLayout]
│   ├── filterScrollArea (255px)          ← 左侧滤镜面板
│   │   └── 各 QCheckBox + QSlider + QComboBox
│   ├── centerWidget                       ← 中央区域
│   │   ├── videoWidget [QHBoxLayout]
│   │   │   ├── grp_originalVideo → lbl_originalVideo (QLabel)
│   │   │   └── grp_processedVideo → lbl_processedVideo (QLabel)
│   │   └── grp_detResults → tbl_detectionResults (QTableWidget)
│   └── detScrollArea (240px)             ← 右侧检测面板
│       └── chk_detection + grp_detSettings
└── QStatusBar
    └── [永久] m_lblFps | m_lblResolution | m_lblDetCount
```

---

## 3. 类设计

### 3.1 MainWindow

已在 `src/ui/mainwindow.h` 实现骨架，后续需补充：

```cpp
// 新增（与 VideoController 对接）
public slots:
    // 接收新帧（在工作线程中 emit，通过 Qt::QueuedConnection 传入 UI 线程）
    void onFrameReady(const cv::Mat& original,
                      const cv::Mat& processed,
                      const DetectionList& detections);
    void onFpsUpdated(double fps);
    void onResolutionChanged(int w, int h);
    void onSourceError(const QString& msg);
    void onRecordingStateChanged(bool recording);

signals:
    // 向 VideoController 发出用户意图
    void openCameraRequested(int deviceIndex);
    void openFileRequested(const QString& path);
    void openScreenRequested(QRect region);
    void playPauseRequested();
    void stopRequested();
    void screenshotRequested();
    void recordToggleRequested();
    void filterParamsChanged(const QString& filterId, const QVariantMap& params);
    void detectionParamsChanged(float confThresh, float nmsThresh);
    void modelLoadRequested(const QString& modelPath);
```

---

### 3.2 VideoDisplay（视频显示控件）

为了将 `cv::Mat` 高效显示到 `QLabel`，封装独立工具函数：

```cpp
// src/ui/VideoDisplay.h
#pragma once
#include <QImage>
#include <QLabel>
#include <opencv2/core.hpp>

class VideoDisplay {
public:
    // 将 cv::Mat (BGR) 转为 QPixmap 并设置到 label，保持纵横比缩放
    static void renderFrame(QLabel* label, const cv::Mat& frame);

private:
    // BGR cv::Mat → QImage (RGB888)，零拷贝版本（frame 需保持有效）
    static QImage matToQImage(const cv::Mat& mat);
};
```

**实现要点：**

```cpp
QImage VideoDisplay::matToQImage(const cv::Mat& mat) {
    // mat: BGR uint8 连续内存
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    // 构造 QImage 时传入 rgb.data 指针；rgb 在此函数返回前需保持存活
    // 故返回 QImage::copy() 或使用 QImage(data, w, h, step, Format_RGB888).copy()
    return QImage(rgb.data, rgb.cols, rgb.rows,
                  static_cast<int>(rgb.step),
                  QImage::Format_RGB888).copy();
}

void VideoDisplay::renderFrame(QLabel* label, const cv::Mat& frame) {
    if (frame.empty()) return;
    QImage img = matToQImage(frame);
    QPixmap pixmap = QPixmap::fromImage(img).scaled(
        label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(pixmap);
}
```

> **性能提示：** `cv::cvtColor` + `QImage::copy` 在 1080p 约 3~5ms。如追求极低延迟，可使用 `YUV→RGB` 直接转换，或将缩放移至 GPU。

---

### 3.3 CameraSelectDialog（摄像头选择对话框）

```cpp
// src/ui/CameraSelectDialog.h
#pragma once
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include "core/VideoSource/CameraSource.h"

class CameraSelectDialog : public QDialog {
    Q_OBJECT
public:
    explicit CameraSelectDialog(QWidget* parent = nullptr);
    int selectedIndex() const;   // 返回设备索引，-1 表示取消

private:
    QComboBox*   m_combo;
    QPushButton* m_okBtn;
    std::vector<CameraInfo> m_cameras;
};
```

---

### 3.4 ScreenRegionSelector（屏幕区域选择）

```cpp
// src/ui/ScreenRegionSelector.h
#pragma once
#include <QWidget>
#include <QRect>

// 全屏透明覆盖窗口，用户可拖拽选择矩形区域
class ScreenRegionSelector : public QWidget {
    Q_OBJECT
public:
    explicit ScreenRegionSelector(QWidget* parent = nullptr);
    static QRect selectRegion();   // 阻塞直到用户完成选择，返回屏幕坐标矩形

signals:
    void regionSelected(QRect region);

protected:
    void mousePressEvent(QMouseEvent*)   override;
    void mouseMoveEvent(QMouseEvent*)    override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*)        override;
    void keyPressEvent(QKeyEvent*)       override;   // Esc 取消

private:
    QPoint m_origin;
    QRect  m_selection;
    bool   m_selecting = false;
};
```

---

## 4. 信号槽连接图

```
MainWindow                              VideoController
──────────────────────────────────────────────────────────────
openCameraRequested(idx)        ──►  onOpenCamera(idx)
openFileRequested(path)         ──►  onOpenFile(path)
playPauseRequested()            ──►  onPlayPause()
stopRequested()                 ──►  onStop()
filterParamsChanged(id, params) ──►  onFilterParamsChanged(id, p)
detectionParamsChanged(c, n)    ──►  onDetectionParamsChanged(c, n)
modelLoadRequested(path)        ──►  onLoadModel(path)
screenshotRequested()           ──►  onScreenshot()
recordToggleRequested()         ──►  onRecordToggle()

VideoController                 ──►  MainWindow
──────────────────────────────────────────────────────────────
frameReady(orig, proc, dets)    ──►  onFrameReady(...)
fpsUpdated(fps)                 ──►  onFpsUpdated(fps)
resolutionChanged(w, h)         ──►  onResolutionChanged(w, h)
sourceError(msg)                ──►  onSourceError(msg)
recordingStateChanged(bool)     ──►  onRecordingStateChanged(b)
```

所有跨线程信号槽连接均使用 `Qt::QueuedConnection`（VideoController 运行在 QThread 中）。

---

## 5. 检测结果表格更新

```cpp
void MainWindow::onFrameReady(const cv::Mat& original,
                               const cv::Mat& processed,
                               const DetectionList& detections)
{
    // 1. 更新视频帧
    VideoDisplay::renderFrame(ui->lbl_originalVideo, original);
    VideoDisplay::renderFrame(ui->lbl_processedVideo, processed);

    // 2. 更新检测结果表格
    ui->tbl_detectionResults->setRowCount(static_cast<int>(detections.size()));
    for (int i = 0; i < static_cast<int>(detections.size()); ++i) {
        const auto& d = detections[i];
        ui->tbl_detectionResults->setItem(i, 0,
            new QTableWidgetItem(QString::fromStdString(d.label)));
        ui->tbl_detectionResults->setItem(i, 1,
            new QTableWidgetItem(QString::number(d.confidence, 'f', 2)));
        ui->tbl_detectionResults->setItem(i, 2,
            new QTableWidgetItem(
                QString("(%1, %2)").arg(int(d.bbox.x)).arg(int(d.bbox.y))));
        ui->tbl_detectionResults->setItem(i, 3,
            new QTableWidgetItem(
                QString("%1 × %2").arg(int(d.bbox.width)).arg(int(d.bbox.height))));
    }

    // 3. 更新检测数量标签
    m_lblDetCount->setText(
        QString("检测数: %1").arg(detections.size()));
}
```

---

## 6. 主题切换（P2，GUI-07）

使用 Qt QSS（Qt Style Sheets）实现深色/浅色主题切换：

```cpp
// 深色主题字符串（独立 QSS 文件）
// resources/theme_dark.qss
// resources/theme_light.qss

void MainWindow::applyTheme(bool dark) {
    QFile f(dark ? ":/themes/dark.qss" : ":/themes/light.qss");
    f.open(QFile::ReadOnly);
    qApp->setStyleSheet(QString::fromUtf8(f.readAll()));
}
```

---

## 7. 关键性能指标

| 指标 | 目标 |
|------|-----|
| `renderFrame()` 耗时（1080p） | < 8 ms |
| 检测结果表格刷新（100 行） | < 2 ms |
| UI 帧率（无掉帧） | 30 FPS |
| UI 线程 CPU 占用 | < 10% |
