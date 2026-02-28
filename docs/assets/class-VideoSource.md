# 视频输入模块类图与时序图

## 1. 类继承关系

```mermaid
classDiagram
    class VideoSource {
        <<abstract>>
        +open() bool*
        +read(frame Mat&) bool*
        +close() void*
        +isOpened() bool*
        +width() int*
        +height() int*
        +fps() double*
        +description() string*
        +pause() void
        +resume() void
        +seek(posMsec) bool
        +posMsec() double
        +durationMsec() double
    }

    class CameraSource {
        -m_deviceIndex int
        -m_preferW int
        -m_preferH int
        -m_preferFps int
        -m_cap VideoCapture
        -m_actualFps double
        +enumerateDevices(maxProbe) CameraInfo[]$
        +open() bool
        +read(frame) bool
        +close() void
    }

    class FileSource {
        -m_path path
        -m_cap VideoCapture
        -m_paused bool
        -m_fps double
        -m_duration double
        +open() bool
        +read(frame) bool
        +pause() void
        +resume() void
        +seek(ms) bool
        +stepForward() bool
    }

    class ScreenSource {
        -m_region QRect
        -m_fps double
        -m_opened bool
        +open() bool
        +read(frame) bool
        +setRegion(r) void
        -qImageToMat(img) Mat
    }

    class CameraInfo {
        +index int
        +name string
    }

    VideoSource <|-- CameraSource
    VideoSource <|-- FileSource
    VideoSource <|-- ScreenSource
    CameraSource ..> CameraInfo : enumerates
```

---

## 2. 摄像头枚举时序

```mermaid
sequenceDiagram
    actor User
    participant MW as MainWindow
    participant DLG as CameraSelectDialog
    participant CS as CameraSource
    participant CAP as cv::VideoCapture

    User->>MW: 点击"打开摄像头"
    MW->>DLG: new CameraSelectDialog()
    DLG->>CS: CameraSource::enumerateDevices(8)
    loop index 0..7
        CS->>CAP: VideoCapture(index)
        CAP-->>CS: isOpened?
        alt 可用
            CS->>CS: push_back CameraInfo
        end
        CS->>CAP: release()
    end
    CS-->>DLG: vector<CameraInfo>
    DLG-->>User: 显示下拉列表
    User->>DLG: 选择设备 → OK
    DLG-->>MW: selectedIndex()
    MW->>VC: emit openCameraRequested(idx)
```

---

## 3. 文件播放控制时序

```mermaid
sequenceDiagram
    participant VC as VideoController
    participant FS as FileSource
    participant TM as QTimer

    VC->>FS: open()
    FS-->>VC: true / false
    VC->>TM: start(1000/fps ms)

    loop 帧循环
        TM->>VC: timeout → doFrameLoop()
        alt not paused
            VC->>FS: read(frame)
            FS-->>VC: frame
        else paused
            VC->>FS: read(frame) → 返回缓存帧
        end
    end

    VC->>FS: pause()
    note over FS: m_paused = true

    VC->>FS: seek(30000)
    FS->>FS: cap.set(CAP_PROP_POS_MSEC, 30000)

    VC->>FS: stepForward()
    FS->>FS: cap.grab() + cap.retrieve(frame)

    VC->>FS: resume()
    note over FS: m_paused = false

    VC->>FS: close()
    VC->>TM: stop()
```

---

## 4. 屏幕区域选择流程

```mermaid
flowchart TD
    A([用户点击"屏幕捕获"]) --> B[显示 ScreenRegionSelector\n全屏透明覆窗]
    B --> C{用户操作}
    C -->|拖拽选择区域| D[记录起点和矩形]
    D --> E[实时绘制选择框]
    E --> C
    C -->|释放鼠标| F[emit regionSelected\nQRect]
    C -->|按 Esc| G[取消，返回空 QRect]
    F --> H[ScreenSource::setRegion\nscreen.grabWindow 循环]
    G --> I([结束])
    H --> J([开始捕获帧])
```
