#include "FilterPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

FilterPanel::FilterPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    connectSignals();
}

FilterPanel::~FilterPanel() = default;

void FilterPanel::setupUi()
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(6, 6, 6, 6);

    auto* titleLabel = new QLabel(QStringLiteral("图像滤镜"), this);
    titleLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 13px;"));
    mainLayout->addWidget(titleLabel);

    auto* sep0 = new QFrame(this);
    sep0->setFrameShape(QFrame::HLine);
    sep0->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep0);

    m_chkGrayscale = new QCheckBox(QStringLiteral("灰度化"), this);
    mainLayout->addWidget(m_chkGrayscale);

    auto* sep1 = new QFrame(this);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep1);

    m_chkGaussBlur = new QCheckBox(QStringLiteral("高斯模糊"), this);
    mainLayout->addWidget(m_chkGaussBlur);

    m_grpGaussParams = new QGroupBox(this);
    m_grpGaussParams->setFlat(true);
    m_grpGaussParams->setEnabled(false);

    auto* gaussLayout = new QVBoxLayout(m_grpGaussParams);
    gaussLayout->setSpacing(3);
    gaussLayout->setContentsMargins(10, 2, 2, 2);

    m_lblGaussKernelVal = new QLabel(QStringLiteral("核大小: 5"), this);
    gaussLayout->addWidget(m_lblGaussKernelVal);

    m_sldGaussKernel = new QSlider(Qt::Horizontal, this);
    m_sldGaussKernel->setMinimum(1);
    m_sldGaussKernel->setMaximum(15);
    m_sldGaussKernel->setValue(2);
    m_sldGaussKernel->setSingleStep(1);
    m_sldGaussKernel->setTickPosition(QSlider::TicksBelow);
    m_sldGaussKernel->setTickInterval(3);
    gaussLayout->addWidget(m_sldGaussKernel);

    m_lblGaussSigmaVal = new QLabel(QStringLiteral("Sigma: 1.0"), this);
    gaussLayout->addWidget(m_lblGaussSigmaVal);

    m_sldGaussSigma = new QSlider(Qt::Horizontal, this);
    m_sldGaussSigma->setMinimum(0);
    m_sldGaussSigma->setMaximum(50);
    m_sldGaussSigma->setValue(10);
    m_sldGaussSigma->setSingleStep(1);
    gaussLayout->addWidget(m_sldGaussSigma);

    mainLayout->addWidget(m_grpGaussParams);

    auto* sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep2);

    m_chkCanny = new QCheckBox(QStringLiteral("Canny 边缘检测"), this);
    mainLayout->addWidget(m_chkCanny);

    m_grpCannyParams = new QGroupBox(this);
    m_grpCannyParams->setFlat(true);
    m_grpCannyParams->setEnabled(false);

    auto* cannyLayout = new QVBoxLayout(m_grpCannyParams);
    cannyLayout->setSpacing(3);
    cannyLayout->setContentsMargins(10, 2, 2, 2);

    m_lblCannyThresh1Val = new QLabel(QStringLiteral("阈值 1: 50"), this);
    cannyLayout->addWidget(m_lblCannyThresh1Val);

    m_sldCannyThresh1 = new QSlider(Qt::Horizontal, this);
    m_sldCannyThresh1->setMinimum(0);
    m_sldCannyThresh1->setMaximum(300);
    m_sldCannyThresh1->setValue(50);
    cannyLayout->addWidget(m_sldCannyThresh1);

    m_lblCannyThresh2Val = new QLabel(QStringLiteral("阈值 2: 150"), this);
    cannyLayout->addWidget(m_lblCannyThresh2Val);

    m_sldCannyThresh2 = new QSlider(Qt::Horizontal, this);
    m_sldCannyThresh2->setMinimum(0);
    m_sldCannyThresh2->setMaximum(300);
    m_sldCannyThresh2->setValue(150);
    cannyLayout->addWidget(m_sldCannyThresh2);

    mainLayout->addWidget(m_grpCannyParams);

    auto* sep3 = new QFrame(this);
    sep3->setFrameShape(QFrame::HLine);
    sep3->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep3);

    m_chkThreshold = new QCheckBox(QStringLiteral("二值化"), this);
    mainLayout->addWidget(m_chkThreshold);

    m_grpThreshParams = new QGroupBox(this);
    m_grpThreshParams->setFlat(true);
    m_grpThreshParams->setEnabled(false);

    auto* threshLayout = new QVBoxLayout(m_grpThreshParams);
    threshLayout->setSpacing(3);
    threshLayout->setContentsMargins(10, 2, 2, 2);

    m_cmbThreshType = new QComboBox(this);
    m_cmbThreshType->addItem(QStringLiteral("固定阈值"));
    m_cmbThreshType->addItem(QStringLiteral("自适应阈值"));
    m_cmbThreshType->addItem(QStringLiteral("Otsu"));
    threshLayout->addWidget(m_cmbThreshType);

    m_lblThreshValueVal = new QLabel(QStringLiteral("阈值: 127"), this);
    threshLayout->addWidget(m_lblThreshValueVal);

    m_sldThreshValue = new QSlider(Qt::Horizontal, this);
    m_sldThreshValue->setMinimum(0);
    m_sldThreshValue->setMaximum(255);
    m_sldThreshValue->setValue(127);
    threshLayout->addWidget(m_sldThreshValue);

    mainLayout->addWidget(m_grpThreshParams);

    auto* sep4 = new QFrame(this);
    sep4->setFrameShape(QFrame::HLine);
    sep4->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(sep4);

    m_chkClahe = new QCheckBox(QStringLiteral("CLAHE 均衡化"), this);
    mainLayout->addWidget(m_chkClahe);

    m_chkSharpen = new QCheckBox(QStringLiteral("锐化"), this);
    mainLayout->addWidget(m_chkSharpen);

    m_chkBgSub = new QCheckBox(QStringLiteral("背景差分"), this);
    mainLayout->addWidget(m_chkBgSub);

    mainLayout->addStretch();

    setMinimumWidth(240);
    setMaximumWidth(260);
}

void FilterPanel::connectSignals()
{
    connect(m_chkGrayscale, &QCheckBox::toggled, this, &FilterPanel::onGrayscaleToggled);
    connect(m_chkGaussBlur, &QCheckBox::toggled, this, &FilterPanel::onGaussBlurToggled);
    connect(m_sldGaussKernel, &QSlider::valueChanged, this, &FilterPanel::onGaussKernelChanged);
    connect(m_sldGaussSigma, &QSlider::valueChanged, this, &FilterPanel::onGaussSigmaChanged);
    connect(m_chkCanny, &QCheckBox::toggled, this, &FilterPanel::onCannyToggled);
    connect(m_sldCannyThresh1, &QSlider::valueChanged, this, &FilterPanel::onCannyThresh1Changed);
    connect(m_sldCannyThresh2, &QSlider::valueChanged, this, &FilterPanel::onCannyThresh2Changed);
    connect(m_chkThreshold, &QCheckBox::toggled, this, &FilterPanel::onThresholdToggled);
    connect(m_cmbThreshType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FilterPanel::onThreshTypeChanged);
    connect(m_sldThreshValue, &QSlider::valueChanged, this, &FilterPanel::onThreshValueChanged);
    connect(m_chkClahe, &QCheckBox::toggled, this, &FilterPanel::onClaheToggled);
    connect(m_chkSharpen, &QCheckBox::toggled, this, &FilterPanel::onSharpenToggled);
    connect(m_chkBgSub, &QCheckBox::toggled, this, &FilterPanel::onBgSubToggled);
}

void FilterPanel::onGrayscaleToggled(bool checked)
{
    emit filterToggled(QStringLiteral("grayscale"), checked);
}

void FilterPanel::onGaussBlurToggled(bool checked)
{
    m_grpGaussParams->setEnabled(checked);
    emit filterToggled(QStringLiteral("gaussian"), checked);
}

void FilterPanel::onGaussKernelChanged(int value)
{
    m_lblGaussKernelVal->setText(QStringLiteral("核大小: %1").arg(value * 2 + 1));
    emit gaussianParamsChanged(value, m_sldGaussSigma->value() / 10.0);
}

void FilterPanel::onGaussSigmaChanged(int value)
{
    m_lblGaussSigmaVal->setText(QStringLiteral("Sigma: %1").arg(value / 10.0, 0, 'f', 1));
    emit gaussianParamsChanged(m_sldGaussKernel->value(), value / 10.0);
}

void FilterPanel::onCannyToggled(bool checked)
{
    m_grpCannyParams->setEnabled(checked);
    emit filterToggled(QStringLiteral("canny"), checked);
}

void FilterPanel::onCannyThresh1Changed(int value)
{
    m_lblCannyThresh1Val->setText(QStringLiteral("阈值 1: %1").arg(value));
    emit cannyParamsChanged(value, m_sldCannyThresh2->value());
}

void FilterPanel::onCannyThresh2Changed(int value)
{
    m_lblCannyThresh2Val->setText(QStringLiteral("阈值 2: %1").arg(value));
    emit cannyParamsChanged(m_sldCannyThresh1->value(), value);
}

void FilterPanel::onThresholdToggled(bool checked)
{
    m_grpThreshParams->setEnabled(checked);
    emit filterToggled(QStringLiteral("threshold"), checked);
}

void FilterPanel::onThreshTypeChanged(int index)
{
    const bool needManual = (index != 2);
    m_sldThreshValue->setEnabled(needManual);
    m_lblThreshValueVal->setEnabled(needManual);
    emit thresholdParamsChanged(index, m_sldThreshValue->value());
}

void FilterPanel::onThreshValueChanged(int value)
{
    m_lblThreshValueVal->setText(QStringLiteral("阈值: %1").arg(value));
    emit thresholdParamsChanged(m_cmbThreshType->currentIndex(), value);
}

void FilterPanel::onClaheToggled(bool checked)
{
    emit histEqToggled(checked, 40.0);
    emit filterToggled(QStringLiteral("histeq"), checked);
}

void FilterPanel::onSharpenToggled(bool checked)
{
    emit sharpenToggled(checked);
    emit filterToggled(QStringLiteral("sharpen"), checked);
}

void FilterPanel::onBgSubToggled(bool checked)
{
    emit bgSubToggled(checked);
    emit filterToggled(QStringLiteral("bgsub"), checked);
}