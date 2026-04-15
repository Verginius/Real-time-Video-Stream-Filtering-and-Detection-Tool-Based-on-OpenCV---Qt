#pragma once

#include <QWidget>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>

class DetectionPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DetectionPanel(QWidget *parent = nullptr);
    ~DetectionPanel() override;

    QCheckBox* chkDetection() const { return m_chkDetection; }
    QLineEdit* edtModelPath() const { return m_edtModelPath; }
    QPushButton* btnLoadModel() const { return m_btnLoadModel; }
    QSlider* sldConfThresh() const { return m_sldConfThresh; }
    QSlider* sldNmsThresh() const { return m_sldNmsThresh; }
    QSpinBox* spnSkipFrames() const { return m_spnSkipFrames; }

    QLabel* lblConfThreshVal() const { return m_lblConfThreshVal; }
    QLabel* lblNmsThreshVal() const { return m_lblNmsThreshVal; }

    QGroupBox* grpDetSettings() const { return m_grpDetSettings; }

signals:
    void detectionToggled(bool enabled);
    void modelLoadRequested(const QString& modelPath, const QString& labelsPath);
    void confThresholdChanged(float threshold);
    void nmsThresholdChanged(float threshold);
    void skipFramesChanged(int skipFrames);

private slots:
    void onDetectionToggled(bool checked);
    void onLoadModelClicked();
    void onConfThreshChanged(int value);
    void onNmsThreshChanged(int value);
    void onSkipFramesChanged(int value);

private:
    void setupUi();
    void connectSignals();

    QCheckBox* m_chkDetection = nullptr;
    QGroupBox* m_grpDetSettings = nullptr;
    QLineEdit* m_edtModelPath = nullptr;
    QPushButton* m_btnLoadModel = nullptr;
    QLabel* m_lblConfThreshVal = nullptr;
    QSlider* m_sldConfThresh = nullptr;
    QLabel* m_lblNmsThreshVal = nullptr;
    QSlider* m_sldNmsThresh = nullptr;
    QSpinBox* m_spnSkipFrames = nullptr;
};