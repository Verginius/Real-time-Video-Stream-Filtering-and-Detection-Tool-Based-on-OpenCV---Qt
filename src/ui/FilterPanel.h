#pragma once

#include <QWidget>
#include <QCheckBox>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>

class FilterPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FilterPanel(QWidget *parent = nullptr);
    ~FilterPanel() override;

    QCheckBox* chkGrayscale() const { return m_chkGrayscale; }
    QCheckBox* chkGaussBlur() const { return m_chkGaussBlur; }
    QSlider* sldGaussKernel() const { return m_sldGaussKernel; }
    QSlider* sldGaussSigma() const { return m_sldGaussSigma; }
    QCheckBox* chkCanny() const { return m_chkCanny; }
    QSlider* sldCannyThresh1() const { return m_sldCannyThresh1; }
    QSlider* sldCannyThresh2() const { return m_sldCannyThresh2; }
    QCheckBox* chkThreshold() const { return m_chkThreshold; }
    QComboBox* cmbThreshType() const { return m_cmbThreshType; }
    QSlider* sldThreshValue() const { return m_sldThreshValue; }
    QCheckBox* chkClahe() const { return m_chkClahe; }
    QCheckBox* chkSharpen() const { return m_chkSharpen; }
    QCheckBox* chkBgSub() const { return m_chkBgSub; }

    QLabel* lblGaussKernelVal() const { return m_lblGaussKernelVal; }
    QLabel* lblGaussSigmaVal() const { return m_lblGaussSigmaVal; }
    QLabel* lblCannyThresh1Val() const { return m_lblCannyThresh1Val; }
    QLabel* lblCannyThresh2Val() const { return m_lblCannyThresh2Val; }
    QLabel* lblThreshValueVal() const { return m_lblThreshValueVal; }

    QGroupBox* grpGaussParams() const { return m_grpGaussParams; }
    QGroupBox* grpCannyParams() const { return m_grpCannyParams; }
    QGroupBox* grpThreshParams() const { return m_grpThreshParams; }

signals:
    void filterToggled(const QString& name, bool enabled);
    void gaussianParamsChanged(int kernelSize, double sigma);
    void cannyParamsChanged(double thresh1, double thresh2);
    void thresholdParamsChanged(int type, int value);
    void histEqToggled(bool enabled, double clipLimit);
    void sharpenToggled(bool enabled);
    void bgSubToggled(bool enabled);

private slots:
    void onGrayscaleToggled(bool checked);
    void onGaussBlurToggled(bool checked);
    void onGaussKernelChanged(int value);
    void onGaussSigmaChanged(int value);
    void onCannyToggled(bool checked);
    void onCannyThresh1Changed(int value);
    void onCannyThresh2Changed(int value);
    void onThresholdToggled(bool checked);
    void onThreshTypeChanged(int index);
    void onThreshValueChanged(int value);
    void onClaheToggled(bool checked);
    void onSharpenToggled(bool checked);
    void onBgSubToggled(bool checked);

private:
    void setupUi();
    void connectSignals();

    QCheckBox* m_chkGrayscale = nullptr;
    QCheckBox* m_chkGaussBlur = nullptr;
    QGroupBox* m_grpGaussParams = nullptr;
    QSlider* m_sldGaussKernel = nullptr;
    QSlider* m_sldGaussSigma = nullptr;
    QLabel* m_lblGaussKernelVal = nullptr;
    QLabel* m_lblGaussSigmaVal = nullptr;

    QCheckBox* m_chkCanny = nullptr;
    QGroupBox* m_grpCannyParams = nullptr;
    QSlider* m_sldCannyThresh1 = nullptr;
    QSlider* m_sldCannyThresh2 = nullptr;
    QLabel* m_lblCannyThresh1Val = nullptr;
    QLabel* m_lblCannyThresh2Val = nullptr;

    QCheckBox* m_chkThreshold = nullptr;
    QGroupBox* m_grpThreshParams = nullptr;
    QComboBox* m_cmbThreshType = nullptr;
    QSlider* m_sldThreshValue = nullptr;
    QLabel* m_lblThreshValueVal = nullptr;

    QCheckBox* m_chkClahe = nullptr;
    QCheckBox* m_chkSharpen = nullptr;
    QCheckBox* m_chkBgSub = nullptr;
};