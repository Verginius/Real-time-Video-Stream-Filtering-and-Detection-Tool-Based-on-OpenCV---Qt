#include "VideoController.h"
#include "Filter/GaussianFilter.h"
#include "Filter/CannyFilter.h"
#include "Filter/ThresholdFilter.h"
#include "Filter/HistEqFilter.h"
#include "VideoSource/CameraSource.h"
#include "VideoSource/FileSource.h"
#include "VideoSource/ScreenSource.h"
#include <QDebug>
#include <QThread>
#include <QMetaObject>
#include <chrono>
#include <filesystem>

VideoController::VideoController(QObject* parent)
    : QObject(parent)
{
    m_detectionRunning = true;
    m_detectionThread = std::thread(&VideoController::detectionWorker, this);
}

VideoController::~VideoController() {
    stopFrameTimer();
    if (m_detectionThread.joinable()) {
        m_detectionRunning = false;
        m_detQueueCV.notify_all();
        m_detectionThread.join();
    }
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
}

void VideoController::moveToWorkerThread() {
    m_workerThread = new QThread(this);
    this->moveToThread(m_workerThread);
    
    // Timer will be created and started in the worker thread
    
    m_workerThread->start();
}

void VideoController::onOpenCamera(int deviceIndex) {
    if (QThread::currentThread() != m_workerThread) {
        QMetaObject::invokeMethod(this, "onOpenCamera", Qt::QueuedConnection, Q_ARG(int, deviceIndex));
        return;
    }
    
    auto src = std::make_unique<CameraSource>(deviceIndex);
    openSource(std::move(src));
}

void VideoController::onOpenFile(const QString& path) {
    if (QThread::currentThread() != m_workerThread) {
        QMetaObject::invokeMethod(this, "onOpenFile", Qt::QueuedConnection, Q_ARG(QString, path));
        return;
    }
    
    auto src = std::make_unique<FileSource>(path.toStdString());
    openSource(std::move(src));
}

void VideoController::onOpenScreen(QRect region, double fps) {
    if (QThread::currentThread() != m_workerThread) {
        QMetaObject::invokeMethod(this, "onOpenScreen", Qt::QueuedConnection, Q_ARG(QRect, region), Q_ARG(double, fps));
        return;
    }
    
    auto src = std::make_unique<ScreenSource>(region, fps);
    openSource(std::move(src));
}

void VideoController::openSource(std::unique_ptr<VideoSource> source) {
    stopFrameTimer();
    
    if (m_source) {
        m_source->close();
    }
    
    m_source = std::move(source);
    if (!m_source->open()) {
        emit sourceError("Failed to open video source: " + QString::fromStdString(m_source->description()));
        m_source.reset();
        return;
    }
    
    emit sourceOpened(QString::fromStdString(m_source->description()));
    emit resolutionChanged(m_source->width(), m_source->height());
    
    if (auto fs = dynamic_cast<FileSource*>(m_source.get())) {
        emit durationMsec(fs->durationMsec());
    }
    
    m_paused = false;
    startFrameTimer(m_source->fps());
}

void VideoController::closeSource() {
    stopFrameTimer();
    if (m_source) {
        m_source->close();
        m_source.reset();
        emit sourceClosed();
    }
}

void VideoController::startFrameTimer(double fps) {
    if (!m_frameTimer) {
        m_frameTimer = new QTimer(this);
        connect(m_frameTimer, &QTimer::timeout, this, &VideoController::doFrameLoop);
    }
    const int intervalMs = static_cast<int>(1000.0 / fps);
    m_frameTimer->start(intervalMs);
}

void VideoController::stopFrameTimer() {
    if (m_frameTimer) {
        m_frameTimer->stop();
    }
}

void VideoController::doFrameLoop() {
    if (!m_source || m_paused) return;

    cv::Mat original;
    if (!m_source->read(original)) {
        stopFrameTimer();
        emit sourceError("End of stream or read failed.");
        return;
    }
    
    // Save original for UI or screenshots
    m_lastOrigFrame = original.clone();
    
    // Process filters (parallel processing for better performance)
    cv::Mat processed = m_filterChain.processParallel(original);
    
    // Object Detection - async queue (non-blocking)
    if (m_detectionEnabled && m_frameCounter % m_skipFrames == 0) {
        {
            std::lock_guard<std::mutex> lock(m_detQueueMutex);
            m_detectionQueue.push(processed.clone());
        }
        m_detQueueCV.notify_one();
    }
    
    // Render current/cached detections onto the frame
    {
        std::lock_guard<std::mutex> lock(m_detMutex);
        m_renderer.render(processed, m_latestDetections);
    }
    
    m_lastProcessedFrame = processed.clone();
    
    // Handle Recording
    if (m_recording) {
        m_recorder.writeFrame(m_lastProcessedFrame);
    }
    
    // Emit to UI
    emit frameReady(m_lastOrigFrame.clone(), m_lastProcessedFrame.clone(), m_latestDetections);
    
    // Update FPS Counter
    m_fpsCounter.tick();
    if (m_frameCounter % 30 == 0) {
        emit fpsUpdated(m_fpsCounter.current());
    }
    
    // Position updates for file
    if (auto fs = dynamic_cast<FileSource*>(m_source.get())) {
        emit positionMsec(fs->posMsec());
    }
    
    m_frameCounter++;
}

void VideoController::FpsCounter::tick() {
    auto now = std::chrono::steady_clock::now();
    m_times.push_back(now);
    
    // Keep only last 1 second of times
    while (m_times.size() > 1 && 
           std::chrono::duration_cast<std::chrono::milliseconds>(now - m_times.front()).count() > 1000) {
        m_times.pop_front();
    }
}

double VideoController::FpsCounter::current() const {
    if (m_times.size() < 2) return 0.0;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_times.back() - m_times.front()).count();
    if (duration == 0) return 0.0;
    return (m_times.size() - 1) * 1000.0 / duration;
}

// GUI Playback Controls
void VideoController::onPlayPause() {
    m_paused = !m_paused;
    // ...
}

void VideoController::onStop() {
    m_paused = true;
    // ...
}

void VideoController::onSeek(double posMsec) {
    if (auto fs = dynamic_cast<FileSource*>(m_source.get())) {
        fs->seek(posMsec);
    }
}

// ---------------------------------------------------------
// Filter controls
// ---------------------------------------------------------

void VideoController::onSetFilterEnabled(const QString& filterId, bool enabled) {
    auto f = m_filterChain.find(filterId.toStdString());
    if (f) {
        f->setEnabled(enabled);
    }
}

void VideoController::onSetGaussianParams(int kernelSteps, double sigma) {
    auto f = std::dynamic_pointer_cast<GaussianFilter>(m_filterChain.find("gaussian"));
    if (f) {
        GaussianParams params = f->params();
        params.kernelSize = kernelSteps * 2 + 1; // Convert steps to odd kernel size
        params.sigmaX = sigma;
        params.sigmaY = sigma;
        f->setParams(params);
    }
}

void VideoController::onSetCannyParams(double thresh1, double thresh2) {
    auto f = std::dynamic_pointer_cast<CannyFilter>(m_filterChain.find("canny"));
    if (f) {
        CannyParams params;
        params.threshold1 = thresh1;
        params.threshold2 = thresh2;
        f->setParams(params);
    }
}

void VideoController::onSetThresholdParams(int type, int value) {
    auto f = std::dynamic_pointer_cast<ThresholdFilter>(m_filterChain.find("threshold"));
    if (f) {
        ThresholdParams params;
        params.type = static_cast<ThresholdType>(type);
        params.value = value;
        f->setParams(params);
    }
}

void VideoController::onSetHistEqParams(bool useClahe, double clipLimit) {
    auto f = std::dynamic_pointer_cast<HistEqFilter>(m_filterChain.find("histeq"));
    if (f) {
        HistEqParams params;
        params.useCLAHE = useClahe;
        params.clipLimit = clipLimit;
        f->setParams(params);
    }
}

void VideoController::onSetSharpenParams(double strength, double sigma) {
    // Sharpen filter is not yet implemented in Filter classes list. Leaving blank.
}

void VideoController::onSetBgSubParams(int algo) {
    // BgSub filter is not yet implemented in Filter classes list. Leaving blank.
}

// ---------------------------------------------------------
// Detection controls
// ---------------------------------------------------------

void VideoController::onLoadModel(const QString& modelPath, const QString& labelsPath) {
    bool success = m_detector.loadModel(modelPath.toStdString(), labelsPath.toStdString());
    if (success) {
        emit modelLoaded(true, "Model loaded successfully: " + modelPath);
    } else {
        emit modelLoaded(false, "Failed to load model: " + modelPath);
    }
}

void VideoController::onSetDetectionEnabled(bool enabled) {
    m_detectionEnabled = enabled;
}

void VideoController::onSetConfThreshold(float thresh) {
    m_detector.setConfThreshold(thresh);
}

void VideoController::onSetNmsThreshold(float thresh) {
    m_detector.setNmsThreshold(thresh);
}

void VideoController::onSetSkipFrames(int n) {
    m_skipFrames = std::max(1, n); // Prevent division by zero
}

// ---------------------------------------------------------
// Export controls
// ---------------------------------------------------------

void VideoController::onScreenshot() {
    if (m_lastProcessedFrame.empty()) return;
    
    // ResultExporter has static methods for saving screenshots
    // For now we will just use a default path and save
    std::filesystem::path outputDir = std::filesystem::current_path();
    std::filesystem::path savedPath = ResultExporter::saveScreenshot(m_lastProcessedFrame, outputDir);
    
    if (!savedPath.empty()) {
        emit screenshotSaved(QString::fromStdString(savedPath.string()));
    }
}

void VideoController::onExportDetections(const QString& format) {
    if (m_latestDetections.empty()) return;
    
    // Export functionality usually expects a path.
    // ResultExporter does not have an easy static method for one-time export in the provided API
    // We would need to implement it or use a temporary instance.
    // But since `m_exporter` does not exist in the exact way initialized, we will just stub it 
    // or instantiate a local one if we needed a file.
    // This is a placeholder since the exact API requires `std::filesystem::path filePath` and format at init.
}

void VideoController::onRecordToggle() {
    if (!m_recording) {
        if (!m_source) return;
        
        if (m_recorder.start()) {
            m_recording = true;
            emit recordingStateChanged(true);
        }
    } else {
        m_recording = false;
        std::filesystem::path finalPath = m_recorder.stop();
        emit recordingStateChanged(false);
        emit recordingSaved(QString::fromStdString(finalPath.string()));
    }
}

void VideoController::onSetRecordOutputDir(const QString& dir) {
    // There is no easy way to set output dir after recorder initialization with current API
    // We would need to recreate the recorder or add a setOutputDir method.
}

void VideoController::detectionWorker() {
    while (m_detectionRunning) {
        cv::Mat frame;
        {
            std::unique_lock<std::mutex> lock(m_detQueueMutex);
            m_detQueueCV.wait(lock, [&]{ 
                return !m_detectionQueue.empty() || !m_detectionRunning; 
            });
            if (!m_detectionQueue.empty()) {
                frame = std::move(m_detectionQueue.front());
                m_detectionQueue.pop();
            }
        }
        
        if (!frame.empty() && m_detectionRunning) {
            auto dets = m_detector.detect(frame);
            std::lock_guard<std::mutex> lock(m_detMutex);
            m_latestDetections = std::move(dets);
        }
    }
}


