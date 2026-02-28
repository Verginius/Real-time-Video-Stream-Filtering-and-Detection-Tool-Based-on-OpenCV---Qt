# GUI 模块控件结构与信号槽图

## 1. 控件树结构

```mermaid
graph TD
    MW["MainWindow\n(QMainWindow)"]

    subgraph MENU["菜单栏 (QMenuBar)"]
        MF["文件(&F)\n打开摄像头 / 文件 / 屏幕 / 退出"]
        MV["视图(&V)\n显示滤镜面板 / 检测面板"]
        MH["帮助(&H)\n关于"]
    end

    subgraph TB["工具栏 (QToolBar)"]
        T1["打开摄像头"]
        T2["打开文件..."]
        T3["屏幕捕获"]
        T4["播放/暂停 ☑"]
        T5["停止"]
        T6["截图"]
        T7["录制 ☑"]
    end

    subgraph SB["状态栏 (QStatusBar)"]
        SB1["FPS: -- "]
        SB2["分辨率: --"]
        SB3["检测数: 0"]
    end

    subgraph CW["centralwidget [QHBoxLayout]"]
        subgraph LEFT["filterScrollArea (255px)"]
            L0["图像滤镜 (标题)"]
            L1["☐ 灰度化"]
            L2["☐ 高斯模糊\n  核大小滑块\n  Sigma 滑块"]
            L3["☐ Canny 边缘\n  阈值1 滑块\n  阈值2 滑块"]
            L4["☐ 二值化\n  类型下拉\n  阈值滑块"]
            L5["☐ CLAHE 均衡化"]
            L6["☐ 锐化"]
            L7["☐ 背景差分"]
        end

        subgraph CENTER["centerWidget [QVBoxLayout]"]
            subgraph VIDEO["videoWidget [QHBoxLayout]"]
                OV["grp_originalVideo\n└─ lbl_originalVideo\n   (QLabel 黑底)"]
                PV["grp_processedVideo\n└─ lbl_processedVideo\n   (QLabel 黑底)"]
            end
            subgraph DET["grp_detResults"]
                DT["tbl_detectionResults\n(QTableWidget 4列)\n类别|置信度|位置|大小"]
            end
        end

        subgraph RIGHT["detScrollArea (240px)"]
            R0["目标检测 (标题)"]
            R1["☐ 启用目标检测"]
            R2["grp_detSettings\n  ONNX模型路径 [...]\n  置信度阈值滑块\n  NMS阈值滑块\n  跳帧推理步数"]
        end
    end

    MW --> MENU
    MW --> TB
    MW --> SB
    MW --> CW
```

---

## 2. 主要信号槽连接

```mermaid
flowchart LR
    subgraph GUI["MainWindow (主线程)"]
        direction TB
        A1["actionOpenCamera\n::triggered"]
        A2["actionOpenFile\n::triggered"]
        A3["actionPlayPause\n::triggered"]
        A4["actionStop\n::triggered"]
        A5["actionScreenshot\n::triggered"]
        A6["actionRecord\n::triggered"]
        A7["chk_gaussBlur\n::toggled"]
        A8["sld_gaussKernel\n::valueChanged"]
        A9["chk_detection\n::toggled"]
        A10["sld_confThresh\n::valueChanged"]
        A11["btn_loadModel\n::clicked"]
    end

    subgraph CTRL["VideoController (工作线程)"]
        direction TB
        B1["onOpenCamera()"]
        B2["onOpenFile()"]
        B3["onPlayPause()"]
        B4["onStop()"]
        B5["onScreenshot()"]
        B6["onRecordToggle()"]
        B7["onSetFilterEnabled\n('gaussian', bool)"]
        B8["onSetGaussianParams\n(kernelSize, sigma)"]
        B9["onSetDetectionEnabled\n(bool)"]
        B10["onSetConfThreshold\n(float)"]
        B11["onLoadModel\n(path, labelsPath)"]
    end

    A1 -->|"QueuedConnection"| B1
    A2 -->|"QueuedConnection"| B2
    A3 -->|"QueuedConnection"| B3
    A4 -->|"QueuedConnection"| B4
    A5 -->|"QueuedConnection"| B5
    A6 -->|"QueuedConnection"| B6
    A7 -->|"QueuedConnection"| B7
    A8 -->|"QueuedConnection"| B8
    A9 -->|"QueuedConnection"| B9
    A10 -->|"QueuedConnection"| B10
    A11 -->|"QueuedConnection"| B11

    subgraph BACK["VideoController → MainWindow"]
        C1["frameReady(orig,proc,dets)"]
        C2["fpsUpdated(fps)"]
        C3["sourceError(msg)"]
        C4["recordingStateChanged(bool)"]
        C5["screenshotSaved(path)"]
    end

    C1 -->|"QueuedConnection"| D1["onFrameReady()\n→ renderFrame × 2\n→ 刷新检测表格"]
    C2 -->|"QueuedConnection"| D2["m_lblFps->setText(...)"]
    C3 -->|"QueuedConnection"| D3["QMessageBox::warning(...)"]
    C4 -->|"QueuedConnection"| D4["actionRecord->setText(...)"]
    C5 -->|"QueuedConnection"| D5["statusBar->showMessage(...)"]
```

---

## 3. 滤镜参数滑块联动逻辑

```mermaid
stateDiagram-v2
    [*] --> FilterOff : 初始化

    FilterOff : chk = false\n参数面板 disabled
    FilterOn  : chk = true\n参数面板 enabled

    FilterOff --> FilterOn : chk_toggled(true)
    FilterOn  --> FilterOff : chk_toggled(false)

    FilterOn --> SliderDrag : 用户拖动滑块
    SliderDrag --> LabelUpdate : valueChanged(v)
    LabelUpdate --> ParamSignal : emit filterParamsChanged()
    ParamSignal --> ControllerUpdate : VideoController::onSetXxxParams()
    ControllerUpdate --> FilterOn : 下一帧生效
```

---

## 4. 视频帧渲染流程

```mermaid
flowchart TD
    IN(["cv::Mat frame\n(工作线程)"])
    CLN["frame.clone()\n(进入 GUI 线程)"]
    CVT["cvtColor BGR→RGB"]
    QI["QImage::fromData\n(Format_RGB888)"]
    CPY["QImage::copy()\n确保内存独立"]
    PIX["QPixmap::fromImage()"]
    SCL["scaled(label.size()\nKeepAspectRatio\nSmoothTransformation)"]
    SET["label->setPixmap(pixmap)"]
    OUT(["屏幕显示"])

    IN --> CLN --> CVT --> QI --> CPY --> PIX --> SCL --> SET --> OUT

    style CLN fill:#fff9c4,stroke:#f9a825
    note1["保证跨线程安全"] -.-> CLN
    note2["约 3~5ms (1080p)"] -.-> CVT
    note3["保持纵横比\n约 1~2ms"] -.-> SCL
```

---

## 5. 主题切换（P2）

```mermaid
flowchart LR
    A([用户切换主题]) --> B{深色/浅色}
    B -->|深色| C["加载 :/themes/dark.qss"]
    B -->|浅色| D["加载 :/themes/light.qss"]
    C & D --> E["qApp->setStyleSheet(qss)"]
    E --> F(["全局样式立即生效"])
```
