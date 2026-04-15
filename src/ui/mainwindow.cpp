#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "VideoDisplay.h"
#include "../core/VideoController.h"
#include <QThread>
#include <QDebug>


#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupStatusBar();
    updateAllParamLabels();

    // 初始化控制器
    m_controller = new VideoController();    
    m_controllerThread = m_controller->moveToWorkerThread();
    //m_controller->moveToThread(m_controllerThread);
    
    // 连接跨线程信号
    connect(m_controllerThread, &QThread::finished, m_controller, &QObject::deleteLater);
    
    // Controller -> GUI
    connect(m_controller, &VideoController::frameReady, this, &MainWindow::onFrameReady, Qt::QueuedConnection);
    connect(m_controller, &VideoController::fpsUpdated, this, &MainWindow::onFpsUpdated, Qt::QueuedConnection);
    connect(m_controller, &VideoController::resolutionChanged, this, &MainWindow::onResolutionChanged, Qt::QueuedConnection);
    connect(m_controller, &VideoController::sourceError, this, &MainWindow::onSourceError, Qt::QueuedConnection);
    connect(m_controller, &VideoController::recordingStateChanged, this, &MainWindow::onRecordingStateChanged, Qt::QueuedConnection);
    connect(m_controller, &VideoController::sourceOpened, this, &MainWindow::onSourceOpened, Qt::QueuedConnection);
    connect(m_controller, &VideoController::sourceClosed, this, &MainWindow::onSourceClosed, Qt::QueuedConnection);
    
    // GUI -> Controller (基本控制)
    connect(this, &MainWindow::openCameraRequested, m_controller, &VideoController::onOpenCamera, Qt::QueuedConnection);
    connect(this, &MainWindow::openFileRequested, m_controller, &VideoController::onOpenFile, Qt::QueuedConnection);
    connect(this, &MainWindow::openScreenRequested, m_controller, &VideoController::onOpenScreen, Qt::QueuedConnection);
    connect(this, &MainWindow::playPauseRequested, m_controller, &VideoController::onPlayPause, Qt::QueuedConnection);
    connect(this, &MainWindow::stopRequested, m_controller, &VideoController::onStop, Qt::QueuedConnection);
    connect(this, &MainWindow::screenshotRequested, m_controller, &VideoController::onScreenshot, Qt::QueuedConnection);
    connect(this, &MainWindow::recordToggleRequested, m_controller, &VideoController::onRecordToggle, Qt::QueuedConnection);
    connect(this, &MainWindow::modelLoadRequested, m_controller, &VideoController::onLoadModel, Qt::QueuedConnection);
    
    // Filter toggle and parameter signals
    connect(ui->chk_grayscale, &QCheckBox::toggled, this, [this](bool b) {
        QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "grayscale"), Q_ARG(bool, b));
    });
    connect(ui->chk_gaussBlur, &QCheckBox::toggled, this, [this](bool b) {
        QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "gaussian"), Q_ARG(bool, b));
    });
    connect(ui->sld_gaussKernel, &QSlider::valueChanged, this, [this](int v) {
        QMetaObject::invokeMethod(m_controller, "onSetGaussianParams", Qt::QueuedConnection, Q_ARG(int, v), Q_ARG(double, ui->sld_gaussSigma->value() / 10.0));
    });
    connect(ui->sld_gaussSigma, &QSlider::valueChanged, this, [this](int v) {
        QMetaObject::invokeMethod(m_controller, "onSetGaussianParams", Qt::QueuedConnection, Q_ARG(int, ui->sld_gaussKernel->value()), Q_ARG(double, v / 10.0));
    });
    
    connect(ui->chk_canny, &QCheckBox::toggled, this, [this](bool b) {
        QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "canny"), Q_ARG(bool, b));
    });
    connect(ui->sld_cannyThresh1, &QSlider::valueChanged, this, [this](int v) {
        QMetaObject::invokeMethod(m_controller, "onSetCannyParams", Qt::QueuedConnection, Q_ARG(double, v), Q_ARG(double, ui->sld_cannyThresh2->value()));
    });
    connect(ui->sld_cannyThresh2, &QSlider::valueChanged, this, [this](int v) {
        QMetaObject::invokeMethod(m_controller, "onSetCannyParams", Qt::QueuedConnection, Q_ARG(double, ui->sld_cannyThresh1->value()), Q_ARG(double, v));
    });
    
    connect(ui->chk_threshold, &QCheckBox::toggled, this, [this](bool b) {
        QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "threshold"), Q_ARG(bool, b));
    });
    connect(ui->cmb_threshType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int v) {
        QMetaObject::invokeMethod(m_controller, "onSetThresholdParams", Qt::QueuedConnection, Q_ARG(int, v), Q_ARG(int, ui->sld_threshValue->value()));
    });
    connect(ui->sld_threshValue, &QSlider::valueChanged, this, [this](int v) {
        QMetaObject::invokeMethod(m_controller, "onSetThresholdParams", Qt::QueuedConnection, Q_ARG(int, ui->cmb_threshType->currentIndex()), Q_ARG(int, v));
    });
    
    connect(ui->chk_clahe, &QCheckBox::toggled, this, [this](bool b) {
        QMetaObject::invokeMethod(m_controller, "onSetHistEqParams", Qt::QueuedConnection, Q_ARG(bool, b), Q_ARG(double, 40.0));
        QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "histeq"), Q_ARG(bool, b));
    });
    connect(ui->chk_sharpen, &QCheckBox::toggled, this, [this](bool b) {
        QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "sharpen"), Q_ARG(bool, b));
    });
    connect(ui->chk_bgSub, &QCheckBox::toggled, this, [this](bool b) {
        QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "bgsub"), Q_ARG(bool, b));
    });
    
    // Detection signals
    connect(ui->chk_detection, &QCheckBox::toggled, this, [this](bool b) {
        QMetaObject::invokeMethod(m_controller, "onSetDetectionEnabled", Qt::QueuedConnection, Q_ARG(bool, b));
    });
    connect(ui->sld_confThresh, &QSlider::valueChanged, this, [this](int v) {
        QMetaObject::invokeMethod(m_controller, "onSetConfThreshold", Qt::QueuedConnection, Q_ARG(float, v / 100.0f));
    });
    connect(ui->sld_nmsThresh, &QSlider::valueChanged, this, [this](int v) {
        QMetaObject::invokeMethod(m_controller, "onSetNmsThreshold", Qt::QueuedConnection, Q_ARG(float, v / 100.0f));
    });
    
    //m_controllerThread->start();


    // 参数面板默认禁用（随对应 checkbox 联动）
    ui->grp_gaussParams->setEnabled(false);
    ui->grp_cannyParams->setEnabled(false);
    ui->grp_threshParams->setEnabled(false);
    ui->grp_detSettings->setEnabled(false);

    ui->actionPlayPause->setChecked(false);
    ui->actionRecord->setChecked(false);
}

MainWindow::~MainWindow()
{

    if (m_controllerThread) {
        m_controllerThread->quit();
        m_controllerThread->wait();
    }
    delete ui;
}

void MainWindow::setupStatusBar()
{
    m_lblFps        = new QLabel(QStringLiteral("FPS: --"), this);
    m_lblResolution = new QLabel(QStringLiteral("分辨率: --"), this);
    m_lblDetCount   = new QLabel(QStringLiteral("检测数: 0"), this);

    m_lblFps->setMinimumWidth(80);
    m_lblResolution->setMinimumWidth(150);
    m_lblDetCount->setMinimumWidth(90);

    auto *sep1 = new QLabel(QStringLiteral(" | "), this);
    auto *sep2 = new QLabel(QStringLiteral(" | "), this);

    statusBar()->addPermanentWidget(m_lblFps);
    statusBar()->addPermanentWidget(sep1);
    statusBar()->addPermanentWidget(m_lblResolution);
    statusBar()->addPermanentWidget(sep2);
    statusBar()->addPermanentWidget(m_lblDetCount);
    statusBar()->showMessage(QStringLiteral("就绪"));
}

void MainWindow::updateAllParamLabels()
{
    // 高斯模糊
    ui->lbl_gaussKernelVal->setText(
        QStringLiteral("核大小: %1").arg(ui->sld_gaussKernel->value() * 2 + 1));
    ui->lbl_gaussSigmaVal->setText(
        QStringLiteral("Sigma: %1").arg(ui->sld_gaussSigma->value() / 10.0, 0, 'f', 1));
    // Canny
    ui->lbl_cannyThresh1Val->setText(
        QStringLiteral("阈值 1: %1").arg(ui->sld_cannyThresh1->value()));
    ui->lbl_cannyThresh2Val->setText(
        QStringLiteral("阈值 2: %1").arg(ui->sld_cannyThresh2->value()));
    // 二值化
    ui->lbl_threshValueVal->setText(
        QStringLiteral("阈值: %1").arg(ui->sld_threshValue->value()));
    // 检测
    ui->lbl_confThreshVal->setText(
        QStringLiteral("置信度阈值: %1%").arg(ui->sld_confThresh->value()));
    ui->lbl_nmsThreshVal->setText(
        QStringLiteral("NMS 阈值: %1%").arg(ui->sld_nmsThresh->value()));
}

void MainWindow::on_actionOpenCamera_triggered()
{
    emit openCameraRequested(0);
}

void MainWindow::on_actionOpenFile_triggered()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("打开视频文件"),
        {},
        QStringLiteral("视频文件 (*.mp4 *.avi *.mkv *.mov);;所有文件 (*.*)"));
    if (!path.isEmpty())
    {
        statusBar()->showMessage(QStringLiteral("已打开: ") + path, 3000);
        emit openFileRequested(path);
    }
        
}

void MainWindow::on_actionOpenScreen_triggered()
{
    emit openScreenRequested(QRect(), 30.0);
}

void MainWindow::on_actionPlayPause_triggered()
{
    emit playPauseRequested();
}

void MainWindow::on_actionStop_triggered()
{
    emit stopRequested();
}

void MainWindow::on_actionScreenshot_triggered()
{
    emit screenshotRequested();
}

void MainWindow::on_actionRecord_triggered()
{
    emit recordToggleRequested();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
        QStringLiteral("关于"),
        QStringLiteral(
            "<h3>实时视频流滤镜与检测工具</h3>"
            "<p>基于 <b>OpenCV</b> + <b>Qt 6</b> 构建</p>"
            "<p>版本: 0.1.0</p>"));
}

void MainWindow::on_actionShowFilter_triggered(bool checked)
{
    ui->filterScrollArea->setVisible(checked);
}

void MainWindow::on_actionShowDetection_triggered(bool checked)
{
    ui->detScrollArea->setVisible(checked);
}

void MainWindow::on_chk_grayscale_toggled(bool checked)
{
    QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "grayscale"), Q_ARG(bool, checked));
}

void MainWindow::on_chk_gaussBlur_toggled(bool checked)
{
    ui->grp_gaussParams->setEnabled(checked);
    QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "gaussian"), Q_ARG(bool, checked));
}

void MainWindow::on_sld_gaussKernel_valueChanged(int value)
{
    ui->lbl_gaussKernelVal->setText(
        QStringLiteral("核大小: %1").arg(value * 2 + 1));
    QMetaObject::invokeMethod(m_controller, "onSetGaussianParams", Qt::QueuedConnection, Q_ARG(int, value), Q_ARG(double, ui->sld_gaussSigma->value() / 10.0));
}

void MainWindow::on_sld_gaussSigma_valueChanged(int value)
{
    ui->lbl_gaussSigmaVal->setText(
        QStringLiteral("Sigma: %1").arg(value / 10.0, 0, 'f', 1));
    QMetaObject::invokeMethod(m_controller, "onSetGaussianParams", Qt::QueuedConnection, Q_ARG(int, ui->sld_gaussKernel->value()), Q_ARG(double, value / 10.0));
}

void MainWindow::on_chk_canny_toggled(bool checked)
{
    ui->grp_cannyParams->setEnabled(checked);
    QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "canny"), Q_ARG(bool, checked));
}

void MainWindow::on_sld_cannyThresh1_valueChanged(int value)
{
    ui->lbl_cannyThresh1Val->setText(QStringLiteral("阈值 1: %1").arg(value));
    QMetaObject::invokeMethod(m_controller, "onSetCannyParams", Qt::QueuedConnection, Q_ARG(double, value), Q_ARG(double, ui->sld_cannyThresh2->value()));
}

void MainWindow::on_sld_cannyThresh2_valueChanged(int value)
{
    ui->lbl_cannyThresh2Val->setText(QStringLiteral("阈值 2: %1").arg(value));
    QMetaObject::invokeMethod(m_controller, "onSetCannyParams", Qt::QueuedConnection, Q_ARG(double, ui->sld_cannyThresh1->value()), Q_ARG(double, value));
}

void MainWindow::on_chk_threshold_toggled(bool checked)
{
    ui->grp_threshParams->setEnabled(checked);
    QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "threshold"), Q_ARG(bool, checked));
}

void MainWindow::on_cmb_threshType_currentIndexChanged(int index)
{
    // Otsu (index=2) 不需要手动阈值
    const bool needManual = (index != 2);
    ui->sld_threshValue->setEnabled(needManual);
    ui->lbl_threshValueVal->setEnabled(needManual);
    QMetaObject::invokeMethod(m_controller, "onSetThresholdParams", Qt::QueuedConnection, Q_ARG(int, index), Q_ARG(int, ui->sld_threshValue->value()));
}

void MainWindow::on_sld_threshValue_valueChanged(int value)
{
    ui->lbl_threshValueVal->setText(QStringLiteral("阈值: %1").arg(value));
    QMetaObject::invokeMethod(m_controller, "onSetThresholdParams", Qt::QueuedConnection, Q_ARG(int, ui->cmb_threshType->currentIndex()), Q_ARG(int, value));
}

void MainWindow::on_chk_clahe_toggled(bool checked)
{
    QMetaObject::invokeMethod(m_controller, "onSetHistEqParams", Qt::QueuedConnection, Q_ARG(bool, checked), Q_ARG(double, 40.0));
    QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "histeq"), Q_ARG(bool, checked));
}

void MainWindow::on_chk_sharpen_toggled(bool checked)
{
    QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "sharpen"), Q_ARG(bool, checked));
}

void MainWindow::on_chk_bgSub_toggled(bool checked)
{
    QMetaObject::invokeMethod(m_controller, "onSetFilterEnabled", Qt::QueuedConnection, Q_ARG(QString, "bgsub"), Q_ARG(bool, checked));
}

void MainWindow::on_chk_detection_toggled(bool checked)
{
    ui->grp_detSettings->setEnabled(checked);
    QMetaObject::invokeMethod(m_controller, "onSetDetectionEnabled", Qt::QueuedConnection, Q_ARG(bool, checked));
}

void MainWindow::on_btn_loadModel_clicked()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("加载 ONNX 模型"),
        {},
        QStringLiteral("ONNX 模型 (*.onnx);;所有文件 (*.*)"));
    if (!path.isEmpty()) {
        ui->edt_modelPath->setText(path);
        statusBar()->showMessage(QStringLiteral("模型已加载: ") + path, 3000);
    }
}

void MainWindow::on_sld_confThresh_valueChanged(int value)
{
    ui->lbl_confThreshVal->setText(QStringLiteral("置信度阈值: %1%").arg(value));
    QMetaObject::invokeMethod(m_controller, "onSetConfThreshold", Qt::QueuedConnection, Q_ARG(float, value / 100.0f));
}

void MainWindow::on_sld_nmsThresh_valueChanged(int value)
{
    ui->lbl_nmsThreshVal->setText(QStringLiteral("NMS 阈值: %1%").arg(value));
    QMetaObject::invokeMethod(m_controller, "onSetNmsThreshold", Qt::QueuedConnection, Q_ARG(float, value / 100.0f));
}

void MainWindow::onFrameReady(const cv::Mat& original, const cv::Mat& processed, const DetectionList& detections)
{
    VideoDisplay::renderFrame(ui->lbl_originalVideo, original);
    VideoDisplay::renderFrame(ui->lbl_processedVideo, processed);

    ui->tbl_detectionResults->setRowCount(static_cast<int>(detections.size()));
    for (int i = 0; i < static_cast<int>(detections.size()); ++i) {
        const auto& d = detections[i];
        ui->tbl_detectionResults->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(d.label)));
        ui->tbl_detectionResults->setItem(i, 1, new QTableWidgetItem(QString::number(d.confidence, 'f', 2)));
        ui->tbl_detectionResults->setItem(i, 2, new QTableWidgetItem(QString("(%1, %2)").arg(int(d.bbox.x)).arg(int(d.bbox.y))));
        ui->tbl_detectionResults->setItem(i, 3, new QTableWidgetItem(QString("%1 x %2").arg(int(d.bbox.width)).arg(int(d.bbox.height))));
    }

    m_lblDetCount->setText(QString("检测数: %1").arg(detections.size()));
}

void MainWindow::onFpsUpdated(double fps)
{
    m_lblFps->setText(QString("FPS: %1").arg(fps, 0, 'f', 1));
}

void MainWindow::onResolutionChanged(int w, int h)
{
    m_lblResolution->setText(QString("分辨率: %1x%2").arg(w).arg(h));
}

void MainWindow::onSourceError(const QString& msg)
{
    QMessageBox::critical(this, "源错误", msg);
    on_actionStop_triggered();
}

void MainWindow::onRecordingStateChanged(bool recording)
{
    m_isRecording = recording;
    ui->actionRecord->setChecked(recording);
    ui->actionRecord->setText(recording ? "停止录制" : "录制");
    statusBar()->showMessage(recording ? "录制中..." : "录制已停止", 2000);
}

void MainWindow::onSourceOpened(const QString& desc)
{
    statusBar()->showMessage(QString("已打开: %1").arg(desc), 3000);
    ui->actionPlayPause->setChecked(true);
    ui->actionPlayPause->setText("暂停");
    m_isPlaying = true;
}

void MainWindow::onSourceClosed()
{
    statusBar()->showMessage("源已关闭", 2000);
    ui->lbl_originalVideo->clear();
    ui->lbl_processedVideo->clear();
    ui->lbl_originalVideo->setText("无信号");
    ui->lbl_processedVideo->setText("无信号");
    ui->tbl_detectionResults->setRowCount(0);
}
