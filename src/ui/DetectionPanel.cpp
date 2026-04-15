#include "DetectionPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QFileDialog>

DetectionPanel::DetectionPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    connectSignals();
}

DetectionPanel::~DetectionPanel() = default;

void DetectionPanel::setupUi()
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(6, 6, 6, 6);

    auto* titleLabel = new QLabel(QStringLiteral("目标检测"), this);
    titleLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 13px;"));
    mainLayout->addWidget(titleLabel);

    auto* sep0 = new QFrame(this);
    sep0->setFrameShape(QFrame::HLine);
    sep0->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep0);

    m_chkDetection = new QCheckBox(QStringLiteral("启用目标检测"), this);
    mainLayout->addWidget(m_chkDetection);

    m_grpDetSettings = new QGroupBox(QStringLiteral("模型设置"), this);
    m_grpDetSettings->setEnabled(false);

    auto* settingsLayout = new QVBoxLayout(m_grpDetSettings);
    settingsLayout->setSpacing(4);
    settingsLayout->setContentsMargins(6, 14, 6, 6);

    auto* modelLabel = new QLabel(QStringLiteral("ONNX 模型:"), this);
    settingsLayout->addWidget(modelLabel);

    auto* modelPathLayout = new QHBoxLayout();
    modelPathLayout->setSpacing(3);

    m_edtModelPath = new QLineEdit(this);
    m_edtModelPath->setPlaceholderText(QStringLiteral("模型路径 (.onnx)"));
    m_edtModelPath->setReadOnly(true);
    modelPathLayout->addWidget(m_edtModelPath);

    m_btnLoadModel = new QPushButton(QStringLiteral("..."), this);
    m_btnLoadModel->setMaximumWidth(30);
    m_btnLoadModel->setToolTip(QStringLiteral("加载 ONNX 模型文件"));
    modelPathLayout->addWidget(m_btnLoadModel);

    settingsLayout->addLayout(modelPathLayout);

    auto* sep1 = new QFrame(this);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    settingsLayout->addWidget(sep1);

    m_lblConfThreshVal = new QLabel(QStringLiteral("置信度阈值: 50%"), this);
    settingsLayout->addWidget(m_lblConfThreshVal);

    m_sldConfThresh = new QSlider(Qt::Horizontal, this);
    m_sldConfThresh->setMinimum(1);
    m_sldConfThresh->setMaximum(99);
    m_sldConfThresh->setValue(50);
    settingsLayout->addWidget(m_sldConfThresh);

    m_lblNmsThreshVal = new QLabel(QStringLiteral("NMS 阈值: 45%"), this);
    settingsLayout->addWidget(m_lblNmsThreshVal);

    m_sldNmsThresh = new QSlider(Qt::Horizontal, this);
    m_sldNmsThresh->setMinimum(1);
    m_sldNmsThresh->setMaximum(99);
    m_sldNmsThresh->setValue(45);
    settingsLayout->addWidget(m_sldNmsThresh);

    auto* sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setFrameShadow(QFrame::Sunken);
    settingsLayout->addWidget(sep2);

    auto* skipFramesLayout = new QHBoxLayout();
    skipFramesLayout->setSpacing(3);

    auto* skipFramesLabel = new QLabel(QStringLiteral("跳帧推理:"), this);
    skipFramesLayout->addWidget(skipFramesLabel);

    m_spnSkipFrames = new QSpinBox(this);
    m_spnSkipFrames->setToolTip(QStringLiteral("每隔 N 帧执行一次推理"));
    m_spnSkipFrames->setMinimum(1);
    m_spnSkipFrames->setMaximum(30);
    m_spnSkipFrames->setValue(3);
    m_spnSkipFrames->setSuffix(QStringLiteral(" 帧"));
    skipFramesLayout->addWidget(m_spnSkipFrames);
    skipFramesLayout->addStretch();

    settingsLayout->addLayout(skipFramesLayout);

    mainLayout->addWidget(m_grpDetSettings);

    mainLayout->addStretch();

    setMinimumWidth(240);
    setMaximumWidth(260);
}

void DetectionPanel::connectSignals()
{
    connect(m_chkDetection, &QCheckBox::toggled, this, &DetectionPanel::onDetectionToggled);
    connect(m_btnLoadModel, &QPushButton::clicked, this, &DetectionPanel::onLoadModelClicked);
    connect(m_sldConfThresh, &QSlider::valueChanged, this, &DetectionPanel::onConfThreshChanged);
    connect(m_sldNmsThresh, &QSlider::valueChanged, this, &DetectionPanel::onNmsThreshChanged);
    connect(m_spnSkipFrames, QOverload<int>::of(&QSpinBox::valueChanged), this, &DetectionPanel::onSkipFramesChanged);
}

void DetectionPanel::onDetectionToggled(bool checked)
{
    m_grpDetSettings->setEnabled(checked);
    emit detectionToggled(checked);
}

void DetectionPanel::onLoadModelClicked()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("加载 ONNX 模型"),
        {},
        QStringLiteral("ONNX 模型 (*.onnx);;所有文件 (*.*)"));
    if (!path.isEmpty()) {
        m_edtModelPath->setText(path);
        emit modelLoadRequested(path, QString());
    }
}

void DetectionPanel::onConfThreshChanged(int value)
{
    m_lblConfThreshVal->setText(QStringLiteral("置信度阈值: %1%").arg(value));
    emit confThresholdChanged(value / 100.0f);
}

void DetectionPanel::onNmsThreshChanged(int value)
{
    m_lblNmsThreshVal->setText(QStringLiteral("NMS 阈值: %1%").arg(value));
    emit nmsThresholdChanged(value / 100.0f);
}

void DetectionPanel::onSkipFramesChanged(int value)
{
    emit skipFramesChanged(value);
}