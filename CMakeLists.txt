cmake_minimum_required(VERSION 3.16)

project(RVSFDT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++20 标准 (移除 -fcoroutines，MinGW 支持不稳定)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -DTBB_USE_PROFILING_TOOLS=0 -DQTD_NO_EMIT")
add_compile_options(-DTBB_USE_PROFILING_TOOLS=0 -DQTD_NO_EMIT)

# ============== 路径配置 ==============
set(Qt6_DIR "D:/Qt/6.10.2/mingw_64/lib/cmake/Qt6") 
set(CMAKE_PREFIX_PATH "D:/Qt/6.10.2/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(CUDA_ROOT "D:/NVIDIA GPU Computing Toolkit/CUDA/v12.6")
set(TENSORRT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/TensorRT-10.16.1.11")
set(CUDNN_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/cudnn-9.2.0.82_cuda12")
set(TBB_ROOT "D:/TBB-MinGW")

# ============== 依赖查找 ==============
# OpenCV (MinGW 版本)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
find_package(OpenCV REQUIRED)

# Qt (MinGW)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# TBB (手动配置路径)
set(TBB_FOUND TRUE)
set(TBB_INCLUDE_DIR "${TBB_ROOT}/include")
set(TBB_LIBRARY_DIR "${TBB_ROOT}/lib")

# TensorRT (手动配置)
set(TensorRT_FOUND TRUE)
set(TensorRT_INCLUDE_DIRS "${TENSORRT_ROOT}/include")
set(TensorRT_LIBRARY_DIRS "${TENSORRT_ROOT}/lib")
set(HAVE_TENSORRT TRUE)

# CUDA (手动配置)
set(CUDA_FOUND TRUE)
set(CUDA_INCLUDE_DIRS "${CUDA_ROOT}/include")
set(CUDA_LIBRARY_DIRS "${CUDA_ROOT}/lib/x64")
set(CUDA_LIBRARIES "${CUDA_ROOT}/lib/x64/cudart.lib")

message(STATUS "TensorRT root: ${TENSORRT_ROOT}")
message(STATUS "cuDNN root: ${CUDNN_ROOT}")
message(STATUS "TBB root: ${TBB_ROOT}")
message(STATUS "CUDA root: ${CUDA_ROOT}")

# ============== 源文件 ==============
set(PROJECT_SOURCES
    src/main.cpp
    src/core/VideoController.h
    src/core/VideoController.cpp
    src/core/VideoSource/VideoSource.h
    src/core/VideoSource/CameraSource.h
    src/core/VideoSource/CameraSource.cpp
    src/core/VideoSource/FileSource.h
    src/core/VideoSource/FileSource.cpp
    src/core/VideoSource/ScreenSource.h
    src/core/VideoSource/ScreenSource.cpp
    src/core/Filter/FilterBase.h
    src/core/Filter/FilterChain.h
    src/core/Filter/FilterChain.cpp
    src/core/Filter/GrayscaleFilter.h
    src/core/Filter/GrayscaleFilter.cpp
    src/core/Filter/GaussianFilter.h
    src/core/Filter/GaussianFilter.cpp
    src/core/Filter/CannyFilter.h
    src/core/Filter/CannyFilter.cpp
    src/core/Filter/ThresholdFilter.h
    src/core/Filter/ThresholdFilter.cpp
    src/core/Filter/HistEqFilter.h
    src/core/Filter/HistEqFilter.cpp
    src/core/Detection/Detection.h
    src/core/Detection/DetectorBase.h
    src/core/Detection/LabelMap.h
    src/core/Detection/YOLODetector.h
    src/core/Detection/YOLODetector.cpp
    src/core/Detection/DetectionRenderer.h
    src/core/Detection/DetectionRenderer.cpp
    src/core/Detection/DetectionBackend.h
    src/core/Detection/DetectionBackend.cpp
    src/core/Detection/TensorRTBackend.h
    src/core/Detection/TensorRTBackend.cpp
    src/core/Export/VideoRecorder.h
    src/core/Export/VideoRecorder.cpp
    src/core/Export/ResultExporter.h
    src/core/Export/ResultExporter.cpp
    src/core/Async/FramePipeline.h
    src/core/Async/FramePipeline.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.cpp
    src/ui/mainwindow.ui
    src/ui/VideoDisplay.h
    src/ui/VideoDisplay.cpp
    src/ui/FilterPanel.h
    src/ui/FilterPanel.cpp
    src/ui/DetectionPanel.h
    src/ui/DetectionPanel.cpp
)

# ============== 目标配置 ==============
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(RVSFDT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(RVSFDT ${PROJECT_SOURCES})
endif()

# ============== 包含目录 ==============
target_include_directories(RVSFDT PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${OpenCV_INCLUDE_DIRS}
    ${TensorRT_INCLUDE_DIRS}
    ${CUDNN_ROOT}/include
    ${TBB_INCLUDE_DIR}
    ${CUDA_INCLUDE_DIRS}
)

# ============== 链接库 ==============
# TensorRT MinGW 导入库
set(TENSORRT_LIBS
    "${TENSORRT_ROOT}/lib/libnvinfer_10.dll.a"
    "${TENSORRT_ROOT}/lib/libnvinfer_plugin_10.dll.a"
    "${TENSORRT_ROOT}/lib/libnvinfer_lean_10.dll.a"
    "${TENSORRT_ROOT}/lib/libnvonnxparser_10.dll.a"
)

# cuDNN 库
set(CUDNN_LIB "${CUDNN_ROOT}/lib/x64/cudnn.lib")

# CUDA 运行时库
set(CUDA_LIB "${CUDA_ROOT}/lib/x64/cudart.lib")

# TBB 库
set(TBB_LIB "${TBB_ROOT}/lib/libtbb12.lib.a")
set(TBB_LIB "${TBB_ROOT}/lib/libtbbmalloc.lib.a")
set(TBB_LIB "${TBB_ROOT}/lib/libtbbmalloc_proxy.lib.a")

target_link_libraries(RVSFDT PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OpenCV_LIBS}
    ${TENSORRT_LIBS}
    ${CUDNN_LIB}
    ${CUDA_LIB}
)

# ============== 编译定义 ==============
target_compile_definitions(RVSFDT PRIVATE
    HAVE_TENSORRT=1
    HAVE_CUDNN=1
    HAVE_CUDA=1
    TBB_USE_PROFILING_TOOLS=0
)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RVSFDT)
endif()
set_target_properties(RVSFDT PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RVSFDT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 安装 OpenCV DLL
file(GLOB OPENCV_DLLS
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/bin/*.dll"
)
install(FILES ${OPENCV_DLLS}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 安装 TensorRT DLL 到输出目录
add_custom_command(TARGET RVSFDT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${TENSORRT_ROOT}/bin"
        $<TARGET_FILE_DIR:RVSFDT>
)

# 安装 cuDNN DLL 到输出目录
add_custom_command(TARGET RVSFDT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CUDNN_ROOT}/bin"
        $<TARGET_FILE_DIR:RVSFDT>
)

# 安装 CUDA runtime DLL 到输出目录
add_custom_command(TARGET RVSFDT POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CUDA_ROOT}/bin"
        $<TARGET_FILE_DIR:RVSFDT>
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RVSFDT)
endif()
