# 录制与导出模块流程图

## 1. 类关系图

```mermaid
classDiagram
    class VideoRecorder {
        -m_cfg RecordConfig
        -m_writer VideoWriter
        -m_recording bool
        -m_frameCount size_t
        -m_droppedFrames atomic~size_t~
        -m_currentPath path
        -m_queue deque~Mat~
        -m_queueMutex mutex
        -m_queueCv condition_variable
        -m_stopIo atomic~bool~
        -m_ioThread thread
        +start() bool
        +writeFrame(frame) void
        +stop() path
        +isRecording() bool
        +frameCount() size_t
        +durationSec() double
        +droppedFrames() size_t
        -ioThreadFunc() void
        -generateFilename() string
    }

    class RecordConfig {
        +outputDir path
        +prefix string = "record"
        +fourcc int = mp4v
        +fps double = 30.0
        +maxQueueSize size_t = 8
    }

    class ScreenshotSaver {
        +save(frame, dir, fmt, quality) path$
        +saveTo(frame, filePath, quality) bool$
        -generateFilename(fmt) string$
    }

    class ResultExporter {
        -m_path path
        -m_fmt Format
        -m_ofs ofstream
        -m_firstFrame bool
        -m_mutex mutex
        +open() bool
        +appendFrame(ts, detections) void
        +close() void
        +isOpen() bool
    }

    class Format {
        <<enumeration>>
        CSV
        JSON
    }

    VideoRecorder --> RecordConfig
    ResultExporter --> Format
    ResultExporter ..> Detection : exports
```

---

## 2. 录制状态机

```mermaid
stateDiagram-v2
    [*] --> Idle : 初始化

    Idle : 未录制\nisRecording = false

    Recording : 录制中\nisRecording = true\ncurrentPath = xxx.mp4\nI/O 线程运行中

    Idle --> WaitFirstFrame : start()\n启动 I/O 线程
    WaitFirstFrame --> Recording : writeFrame()\n第一帧入队\n→ I/O 线程打开 VideoWriter
    WaitFirstFrame --> Idle : stop() 提前停止\nI/O 线程退出

    Recording --> Flush : stop()\n设置 stopIo=true\n等待队列清空
    Flush --> Idle : I/O 线程 join()\nVideoWriter::release()\nemit recordingSaved(path)

    Recording --> Recording : writeFrame(frame)\n非阻塞入队 (< 0.1ms)\n[队列满则丢弃最老帧\n+ droppedFrames++]

    state WaitFirstFrame {
        note: 等待第一帧以确定分辨率
    }

    state Flush {
        note: 阻塞等待 I/O 线程写完队列中的剩余帧
    }
```

---

## 3. 录制完整时序

```mermaid
sequenceDiagram
    actor User
    participant MW as MainWindow
    participant VC as VideoController
    participant VR as VideoRecorder
    participant IOT as I/O 线程 (VR内部)
    participant CVW as cv::VideoWriter
    participant FS as 文件系统

    User->>MW: 点击"录制"按钮
    MW->>VC: emit recordToggleRequested()
    VC->>VR: start()
    VR->>VR: generateFilename()\n→ "record_20260301_143022.mp4"
    VR->>IOT: 启动 I/O 线程\n(阻塞等待队列非空)
    VR-->>VC: true (准备就绪, 等待第一帧)
    VC->>MW: emit recordingStateChanged(true)
    MW->>MW: actionRecord->setText("停止录制")

    loop 每帧处理 (工作线程, ~33ms/帧)
        VC->>VC: doFrameLoop() → processed
        VC->>VR: writeFrame(processed)
        note over VR: 非阻塞入队 < 0.1ms\n[队列满则丢最老帧]
        VR-->>VC: 立即返回

        alt 队列有帧 (I/O 线程)
            IOT->>VR: 出队取帧
            alt 第一帧
                IOT->>CVW: open(path, fourcc, fps, Size(w,h))
                CVW->>FS: 创建 .mp4 文件
            end
            IOT->>CVW: write(frame)  [约 3~8ms]
            IOT->>VR: frameCount++
        end
    end

    User->>MW: 再次点击"停止录制"
    MW->>VC: emit recordToggleRequested()
    VC->>VR: stop()
    VR->>IOT: 设置 stopIo=true\n通知 queueCv
    VR->>IOT: join() [等待队列 flush]
    IOT->>CVW: write(剩余帧...)
    IOT-->>VR: 退出
    VR->>CVW: release() [flush + close]
    CVW->>FS: 完整写出 mp4
    VR-->>VC: path = "record_20260301_143022.mp4"
    VC->>MW: emit recordingSaved(path)
    VC->>MW: emit recordingStateChanged(false)
    MW->>MW: statusBar→showMessage("录制已保存: ...")
```

---

## 4. 截图保存流程

```mermaid
flowchart TD
    A([用户按 Ctrl+P]) 
        --> B["MainWindow::on_actionScreenshot_triggered()"]
        --> C["emit screenshotRequested()"]
        --> D["VideoController::onScreenshot()"]
        --> E["ScreenshotSaver::save(\n  m_lastProcessedFrame,\n  PicturesLocation/RVSFDT,\n  PNG, 100)"]

    E --> F{目录存在?}
    F -->|否| G["create_directories(dir)"]
    F -->|是| H
    G --> H["generateFilename()\nscreenshot_20260301_143055_123.png"]
    H --> I["cv::imwrite(filePath, frame)"]
    I --> J{成功?}
    J -->|是| K["emit screenshotSaved(path)"]
    J -->|否| L["emit sourceError('截图失败')"]
    K --> M(["statusBar 显示保存路径"])
    L --> N(["QMessageBox 错误提示"])
```

---

## 5. 检测结果导出流程（P2）

```mermaid
flowchart TD
    subgraph CSV["CSV 格式"]
        C1["open()\n写入表头:\ntimestamp_ms,frame_id,label,confidence,x,y,w,h"]
        C2["appendFrame(ts, dets)\n每个 Detection 写一行"]
        C3["close()\nflush + 关闭文件"]
        C1 --> C2 --> C3
    end

    subgraph JSON["JSON 格式"]
        J1["open()\n写入 '['"]
        J2["appendFrame(ts, dets)\n写入一个帧对象\n含 timestamp_ms + detections 数组"]
        J3["close()\n写入 ']' + flush"]
        J1 --> J2 --> J3
    end

    START([开始导出]) --> FMT{选择格式}
    FMT -->|CSV| C1
    FMT -->|JSON| J1

    style C1 fill:#e8f5e9,stroke:#388e3c
    style J1 fill:#e3f2fd,stroke:#1976d2
```

---

## 6. 输出目录规划

```mermaid
graph LR
    ROOT["用户主目录 (~)"]
    ROOT --> VID["Videos/RVSFDT/\n视频录制 (*.mp4)"]
    ROOT --> PIC["Pictures/RVSFDT/\n截图 (*.png / *.jpg)"]
    ROOT --> DOC["Documents/RVSFDT/\n检测结果 (*.csv / *.json)"]

    VID --> V1["record_20260301_143022.mp4"]
    VID --> V2["record_20260301_150011.mp4"]
    PIC --> P1["screenshot_20260301_143055_123.png"]
    DOC --> D1["detections_20260301_143022.csv"]
```

---

## 7. VideoRecorder 内部写队列架构

```mermaid
flowchart TB
    subgraph WT["工作线程（QTimer 帧循环）"]
        A["doFrameLoop()\nprocessed frame"]
        B["VideoRecorder::writeFrame(frame)\n非阻塞入队 < 0.1ms"]
        A --> B
    end

    subgraph VR["VideoRecorder 内部"]
        Q["有界帧队列\nstd::deque&lt;cv::Mat&gt;\n上限 maxQueueSize = 8 帧\n≈ 96MB @ 1080p"]
        DROP["droppedFrames++\n丢弃最老帧"]
        B --> FULL{队列已满?}
        FULL -->|否| Q
        FULL -->|是| DROP
        DROP --> Q
    end

    subgraph IOT["I/O 线程（VR 内部独立线程）"]
        C["阻塞等待 queueCv\n出队取帧"]
        D["cv::VideoWriter::write()\n约 3~8ms / 帧"]
        Q --> C --> D
    end

    subgraph STOP["stop() 调用"]
        E["设置 stopIo = true\n通知 queueCv"]
        F["join() 等待 I/O 线程\n队列 flush 完毕"]
        G["VideoWriter::release()"]
        E --> F --> G
    end

    style Q fill:#e3f2fd,stroke:#1976d2
    style DROP fill:#fce4ec,stroke:#c62828
    style IOT fill:#e8f5e9,stroke:#388e3c
```
