@echo off
setlocal enabledelayedexpansion

echo ========================================
echo TensorRT MinGW Import Library Generator
echo ========================================
echo.

set MINGW_DIR=D:\Qt\Tools\mingw1310_64
set TENSORRT_DIR=D:\NVIDIA GPU Computing Toolkit\TensorRT-10.16.1.11
set CUDNN_DIR=D:\NVIDIA GPU Computing Toolkit\cudnn-9.2.0.82_cuda12
set PROJECT_DIR=E:\projects\Real-time Video Stream Filtering and Detection Tool Based on OpenCV + Qt
set LIBS_DIR=%PROJECT_DIR%\libs

echo [1/5] Creating directory structure...
if not exist "%LIBS_DIR%\TensorRT-10.16.1.11\bin" mkdir "%LIBS_DIR%\TensorRT-10.16.1.11\bin"
if not exist "%LIBS_DIR%\TensorRT-10.16.1.11\include" mkdir "%LIBS_DIR%\TensorRT-10.16.1.11\include"
if not exist "%LIBS_DIR%\TensorRT-10.16.1.11\lib" mkdir "%LIBS_DIR%\TensorRT-10.16.1.11\lib"
if not exist "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\bin" mkdir "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\bin"
if not exist "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\include" mkdir "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\include"
if not exist "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\lib" mkdir "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\lib"
echo Done.

echo [2/5] Copying TensorRT files...
xcopy /Y /Q "%TENSORRT_DIR%\bin\*.dll" "%LIBS_DIR%\TensorRT-10.16.1.11\bin\" 
xcopy /Y /Q "%TENSORRT_DIR%\include\*.h" "%LIBS_DIR%\TensorRT-10.16.1.11\include\"
echo Done.

echo [3/5] Copying cuDNN files...
xcopy /Y /Q "%CUDNN_DIR%\bin\*.dll" "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\bin\" 
xcopy /Y /Q "%CUDNN_DIR%\include\*.h" "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\include\"
xcopy /Y /Q "%CUDNN_DIR%\lib\x64\*.lib" "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\lib\"
echo Done.

echo [4/5] Generating MinGW import libraries...
cd /D "%LIBS_DIR%\TensorRT-10.16.1.11\bin"

set DLL_LIST=nvinfer_10 nvinfer_lean_10 nvinfer_plugin_10 nvinfer_dispatch_10 nvonnxparser_10

for %%d in (%DLL_LIST%) do (
    if exist %%d.dll (
        echo   Processing %%d.dll...
        "%MINGW_DIR%\bin\gendef.exe" %%d.dll
        if exist %%d.def (
            "%MINGW_DIR%\bin\dlltool.exe" -d %%d.def -l ..\lib\lib%%d.dll.a -A
            del /F %%d.def
        )
    ) else (
        echo   Skipping %%d.dll (not found)
    )
)
echo Done.

echo [5/5] Verifying libraries...
echo.
echo TensorRT lib directory:
dir /B "%LIBS_DIR%\TensorRT-10.16.1.11\lib\"
echo.
echo cuDNN lib directory:
dir /B "%LIBS_DIR%\cudnn-9.2.0.82_cuda12\lib\"
echo.

echo ========================================
echo Done! Libraries created successfully.
echo ========================================
