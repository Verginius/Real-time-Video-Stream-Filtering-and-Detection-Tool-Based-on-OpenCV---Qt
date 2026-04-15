#include "ResultExporter.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <iomanip>
#include <sstream>

ResultExporter::ResultExporter(std::filesystem::path filePath, Format fmt)
    : m_path(std::move(filePath)), m_fmt(fmt), m_firstFrame(true)
{
}

ResultExporter::~ResultExporter()
{
    close();
}

bool ResultExporter::open()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_ofs.is_open()) return true;

    if (!std::filesystem::exists(m_path.parent_path())) {
         try {
            std::filesystem::create_directories(m_path.parent_path());
         } catch (...) {
             return false;
         }
    }

    m_ofs.open(m_path, std::ios::out | std::ios::trunc);
    if (!m_ofs.is_open()) return false;

    if (m_fmt == Format::CSV) {
        writeCsvHeader();
    } else if (m_fmt == Format::JSON) {
        m_ofs << "[\n";
    }

    return true;
}

void ResultExporter::appendFrame(std::int64_t timestampMsec, const DetectionList& detections)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_ofs.is_open()) return;

    if (m_fmt == Format::CSV) {
        for (const auto& d : detections) {
            writeCsvRow(timestampMsec, d);
        }
    } else if (m_fmt == Format::JSON) {
        writeJsonFrameOpen(timestampMsec, detections.size());
        for (size_t i = 0; i < detections.size(); ++i) {
            writeJsonDetection(detections[i], i == detections.size() - 1);
        }
        writeJsonFrameClose();
    }
}

void ResultExporter::close()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_ofs.is_open()) {
        if (m_fmt == Format::JSON) {
            writeJsonFooter();
        }
        m_ofs.flush();
        m_ofs.close();
    }
}

bool ResultExporter::isOpen() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_ofs.is_open();
}

void ResultExporter::writeCsvHeader()
{
    m_ofs << "Timestamp_ms,ClassID,Label,Confidence,X,Y,Width,Height\n";
}

void ResultExporter::writeCsvRow(std::int64_t ts, const Detection& d)
{
    m_ofs << ts << ","
          << d.classId << ","
          << "\"" << d.label << "\","
          << d.confidence << ","
          << d.bbox.x << ","
          << d.bbox.y << ","
          << d.bbox.width << ","
          << d.bbox.height << "\n";
}

void ResultExporter::writeJsonFrameOpen(std::int64_t ts, std::size_t count)
{
    if (!m_firstFrame) {
        m_ofs << ",\n";
    }
    m_firstFrame = false;

    m_ofs << "  {\n"
          << "    \"timestamp_ms\": " << ts << ",\n"
          << "    \"num_detections\": " << count << ",\n"
          << "    \"detections\": [\n";
}

void ResultExporter::writeJsonDetection(const Detection& d, bool last)
{
    m_ofs << "      {\n"
          << "        \"class_id\": " << d.classId << ",\n"
          << "        \"label\": \"" << d.label << "\",\n"
          << "        \"confidence\": " << d.confidence << ",\n"
          << "        \"bbox\": [" << d.bbox.x << ", " << d.bbox.y << ", " << d.bbox.width << ", " << d.bbox.height << "]\n"
          << "      }" << (last ? "" : ",") << "\n";
}

void ResultExporter::writeJsonFrameClose()
{
    m_ofs << "    ]\n"
          << "  }";
}

void ResultExporter::writeJsonFooter()
{
    m_ofs << "\n]\n";
}

std::string ResultExporter::generateScreenshotFilename(ImageFormat fmt)
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << "screenshot_";
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    ss << "_" << std::setfill('0') << std::setw(3) << ms.count();
    
    ss << (fmt == ImageFormat::PNG ? ".png" : ".jpg");
    return ss.str();
}

std::filesystem::path ResultExporter::saveScreenshot(
    const cv::Mat& frame,
    const std::filesystem::path& outputDir,
    ImageFormat fmt,
    int jpegQuality)
{
    if (frame.empty()) return {};

    if (!std::filesystem::exists(outputDir)) {
         try {
            std::filesystem::create_directories(outputDir);
         } catch (...) {
             return {};
         }
    }

    std::string filename = generateScreenshotFilename(fmt);
    std::filesystem::path fullPath = outputDir / filename;

    if (saveScreenshotTo(frame, fullPath, jpegQuality)) {
        return fullPath;
    }
    return {};
}

bool ResultExporter::saveScreenshotTo(
    const cv::Mat& frame,
    const std::filesystem::path& filePath,
    int jpegQuality)
{
    if (frame.empty()) return false;

    std::vector<int> params;
    std::string ext = filePath.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".jpg" || ext == ".jpeg") {
        params.push_back(cv::IMWRITE_JPEG_QUALITY);
        params.push_back(jpegQuality);
    } else if (ext == ".png") {
        params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        params.push_back(3); // default compression
    }

    try {
        return cv::imwrite(filePath.string(), frame, params);
    } catch (const cv::Exception& e) {
        std::cerr << "OpenCV error saving screenshot: " << e.what() << std::endl;
        return false;
    }
}