#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ---- 输入源 ----
    void on_actionOpenCamera_triggered();
    void on_actionOpenFile_triggered();
    void on_actionOpenScreen_triggered();
    void on_actionPlayPause_triggered();
    void on_actionStop_triggered();
    void on_actionScreenshot_triggered();
    void on_actionRecord_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();

    // ---- 视图切换 ----
    void on_actionShowFilter_triggered(bool checked);
    void on_actionShowDetection_triggered(bool checked);

    // ---- 滤镜 ----
    void on_chk_grayscale_toggled(bool checked);
    void on_chk_gaussBlur_toggled(bool checked);
    void on_sld_gaussKernel_valueChanged(int value);
    void on_sld_gaussSigma_valueChanged(int value);
    void on_chk_canny_toggled(bool checked);
    void on_sld_cannyThresh1_valueChanged(int value);
    void on_sld_cannyThresh2_valueChanged(int value);
    void on_chk_threshold_toggled(bool checked);
    void on_cmb_threshType_currentIndexChanged(int index);
    void on_sld_threshValue_valueChanged(int value);
    void on_chk_clahe_toggled(bool checked);
    void on_chk_sharpen_toggled(bool checked);
    void on_chk_bgSub_toggled(bool checked);

    // ---- 检测 ----
    void on_chk_detection_toggled(bool checked);
    void on_btn_loadModel_clicked();
    void on_sld_confThresh_valueChanged(int value);
    void on_sld_nmsThresh_valueChanged(int value);

private:
    void setupStatusBar();
    void updateAllParamLabels();

    Ui::MainWindow *ui;

    // 状态栏永久标签
    QLabel *m_lblFps       = nullptr;
    QLabel *m_lblResolution = nullptr;
    QLabel *m_lblDetCount  = nullptr;

    bool m_isPlaying   = false;
    bool m_isRecording = false;
};

#endif // MAINWINDOW_H
