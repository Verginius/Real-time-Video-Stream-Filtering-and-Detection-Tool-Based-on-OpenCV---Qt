#pragma once
#include "core/Detection/Detection.h"
#include <opencv2/core.hpp>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <mutex>
#include <string>

class ResultExporter {
public:
    enum class Format { CSV, JSON };
    enum class ImageFormat { PNG, JPEG };

    explicit ResultExporter(std::filesystem::path filePath, Format fmt);
    ~ResultExporter();   // 自动 flush + close

    // 打开文件，写入 CSV 表头 / JSON 开头
    bool open();

    // 追加一帧的检测结果（含帧时间戳 ms）
    void appendFrame(std::int64_t timestampMsec,
                     const DetectionList& detections);

    // 手动关闭（也可依赖析构）
    void close();

    bool isOpen() const;

    // ── 截图功能（静态方法，不依赖导出文件状态）──────────────
    // 保存帧到指定目录，文件名自动带时间戳；返回最终路径，失败返回空路径
    static std::filesystem::path saveScreenshot(
        const cv::Mat& frame,
        const std::filesystem::path& outputDir,
        ImageFormat fmt    = ImageFormat::PNG,
        int jpegQuality    = 95);

    // 保存到指定完整路径
    static bool saveScreenshotTo(
        const cv::Mat& frame,
        const std::filesystem::path& filePath,
        int jpegQuality = 95);

private:
    void writeCsvHeader();
    void writeCsvRow(std::int64_t ts, const Detection& d);
    void writeJsonFrameOpen(std::int64_t ts, std::size_t count);
    void writeJsonDetection(const Detection& d, bool last);
    void writeJsonFrameClose();
    void writeJsonFooter();
    static std::string generateScreenshotFilename(ImageFormat fmt);

    std::filesystem::path m_path;
    Format                m_fmt;
    std::ofstream         m_ofs;
    bool                  m_firstFrame = true;
    mutable std::mutex    m_mutex;
};