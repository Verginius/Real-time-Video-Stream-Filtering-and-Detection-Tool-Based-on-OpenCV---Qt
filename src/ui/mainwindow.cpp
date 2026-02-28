#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

// ─────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupStatusBar();
    updateAllParamLabels();

    // 参数面板默认禁用（随对应 checkbox 联动）
    ui->grp_gaussParams->setEnabled(false);
    ui->grp_cannyParams->setEnabled(false);
    ui->grp_threshParams->setEnabled(false);
    ui->grp_detSettings->setEnabled(false);

    // PlayPause / Record 初始文字
    ui->actionPlayPause->setChecked(false);
    ui->actionRecord->setChecked(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ─────────────────────────────────────────────────────────
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

// ─────────────────────────────────────────────────────────
// 输入源
// ─────────────────────────────────────────────────────────
void MainWindow::on_actionOpenCamera_triggered()
{
    // TODO: 枚举并打开摄像头
    statusBar()->showMessage(QStringLiteral("打开摄像头 — 待实现"), 2000);
}

void MainWindow::on_actionOpenFile_triggered()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("打开视频文件"),
        {},
        QStringLiteral("视频文件 (*.mp4 *.avi *.mkv *.mov);;所有文件 (*.*)"));
    if (!path.isEmpty())
        statusBar()->showMessage(QStringLiteral("已打开: ") + path, 3000);
}

void MainWindow::on_actionOpenScreen_triggered()
{
    statusBar()->showMessage(QStringLiteral("屏幕捕获 — 待实现"), 2000);
}

void MainWindow::on_actionPlayPause_triggered()
{
    m_isPlaying = ui->actionPlayPause->isChecked();
    ui->actionPlayPause->setText(m_isPlaying ? QStringLiteral("暂停") : QStringLiteral("播放"));
    statusBar()->showMessage(m_isPlaying ? QStringLiteral("播放中…") : QStringLiteral("已暂停"), 1500);
}

void MainWindow::on_actionStop_triggered()
{
    m_isPlaying = false;
    ui->actionPlayPause->setChecked(false);
    ui->actionPlayPause->setText(QStringLiteral("播放"));
    statusBar()->showMessage(QStringLiteral("已停止"), 1500);
}

void MainWindow::on_actionScreenshot_triggered()
{
    // TODO: 将 lbl_processedVideo 的当前帧保存为图片
    statusBar()->showMessage(QStringLiteral("截图已保存"), 2000);
}

void MainWindow::on_actionRecord_triggered()
{
    m_isRecording = ui->actionRecord->isChecked();
    ui->actionRecord->setText(m_isRecording ? QStringLiteral("停止录制") : QStringLiteral("录制"));
    statusBar()->showMessage(
        m_isRecording ? QStringLiteral("录制中…") : QStringLiteral("录制已停止"), 2000);
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

// ─────────────────────────────────────────────────────────
// 视图切换
// ─────────────────────────────────────────────────────────
void MainWindow::on_actionShowFilter_triggered(bool checked)
{
    ui->filterScrollArea->setVisible(checked);
}

void MainWindow::on_actionShowDetection_triggered(bool checked)
{
    ui->detScrollArea->setVisible(checked);
}

// ─────────────────────────────────────────────────────────
// 滤镜槽
// ─────────────────────────────────────────────────────────
void MainWindow::on_chk_grayscale_toggled(bool /*checked*/)
{
    // TODO: 通知处理管线
}

void MainWindow::on_chk_gaussBlur_toggled(bool checked)
{
    ui->grp_gaussParams->setEnabled(checked);
}

void MainWindow::on_sld_gaussKernel_valueChanged(int value)
{
    ui->lbl_gaussKernelVal->setText(
        QStringLiteral("核大小: %1").arg(value * 2 + 1));
}

void MainWindow::on_sld_gaussSigma_valueChanged(int value)
{
    ui->lbl_gaussSigmaVal->setText(
        QStringLiteral("Sigma: %1").arg(value / 10.0, 0, 'f', 1));
}

void MainWindow::on_chk_canny_toggled(bool checked)
{
    ui->grp_cannyParams->setEnabled(checked);
}

void MainWindow::on_sld_cannyThresh1_valueChanged(int value)
{
    ui->lbl_cannyThresh1Val->setText(QStringLiteral("阈值 1: %1").arg(value));
}

void MainWindow::on_sld_cannyThresh2_valueChanged(int value)
{
    ui->lbl_cannyThresh2Val->setText(QStringLiteral("阈值 2: %1").arg(value));
}

void MainWindow::on_chk_threshold_toggled(bool checked)
{
    ui->grp_threshParams->setEnabled(checked);
}

void MainWindow::on_cmb_threshType_currentIndexChanged(int index)
{
    // Otsu (index=2) 不需要手动阈值
    const bool needManual = (index != 2);
    ui->sld_threshValue->setEnabled(needManual);
    ui->lbl_threshValueVal->setEnabled(needManual);
}

void MainWindow::on_sld_threshValue_valueChanged(int value)
{
    ui->lbl_threshValueVal->setText(QStringLiteral("阈值: %1").arg(value));
}

void MainWindow::on_chk_clahe_toggled(bool /*checked*/)
{
    // TODO: 通知处理管线
}

void MainWindow::on_chk_sharpen_toggled(bool /*checked*/)
{
    // TODO: 通知处理管线
}

void MainWindow::on_chk_bgSub_toggled(bool /*checked*/)
{
    // TODO: 通知处理管线
}

// ─────────────────────────────────────────────────────────
// 检测槽
// ─────────────────────────────────────────────────────────
void MainWindow::on_chk_detection_toggled(bool checked)
{
    ui->grp_detSettings->setEnabled(checked);
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
}

void MainWindow::on_sld_nmsThresh_valueChanged(int value)
{
    ui->lbl_nmsThreshVal->setText(QStringLiteral("NMS 阈值: %1%").arg(value));
}
