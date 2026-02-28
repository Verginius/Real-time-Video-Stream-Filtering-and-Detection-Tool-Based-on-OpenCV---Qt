cmake_minimum_required(VERSION 3.16)

project(RVSFDT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
find_package(OpenCV REQUIRED)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(PROJECT_SOURCES
    # 入口
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

    # 核心控制器
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoController.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoController.cpp

    # 视频输入
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoSource/VideoSource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoSource/CameraSource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoSource/CameraSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoSource/FileSource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoSource/FileSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoSource/ScreenSource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/VideoSource/ScreenSource.cpp

    # 滤镜
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/FilterBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/FilterChain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/FilterChain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/GrayscaleFilter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/GrayscaleFilter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/GaussianFilter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/GaussianFilter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/CannyFilter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/CannyFilter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/ThresholdFilter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/ThresholdFilter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/HistEqFilter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Filter/HistEqFilter.cpp

    # 目标检测
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Detection/Detection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Detection/DetectorBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Detection/LabelMap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Detection/YOLODetector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Detection/YOLODetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Detection/DetectionRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Detection/DetectionRenderer.cpp

    # 录制与导出
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Export/VideoRecorder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Export/VideoRecorder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Export/ResultExporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/Export/ResultExporter.cpp

    # UI
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/VideoDisplay.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/VideoDisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/FilterPanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/FilterPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/DetectionPanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/DetectionPanel.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RVSFDT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RVSFDT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(RVSFDT SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RVSFDT
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(RVSFDT PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(RVSFDT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
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

# 安装 OpenCV DLL 到与可执行文件相同的目录
file(GLOB OPENCV_DLLS
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/bin/*.dll"
)
install(FILES ${OPENCV_DLLS}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RVSFDT)
endif()
