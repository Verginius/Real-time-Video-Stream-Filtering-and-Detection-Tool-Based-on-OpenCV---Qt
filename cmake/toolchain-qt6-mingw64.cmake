# Toolchain file for Qt 6.10.2 with MinGW 13.1.0 64-bit on Windows

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# MinGW toolchain root
set(MINGW_ROOT "D:/Qt/Tools/mingw1310_64")

# C / C++ compilers
set(CMAKE_C_COMPILER   "${MINGW_ROOT}/bin/gcc.exe"  CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER "${MINGW_ROOT}/bin/g++.exe"  CACHE FILEPATH "C++ compiler")
set(CMAKE_RC_COMPILER  "${MINGW_ROOT}/bin/windres.exe" CACHE FILEPATH "RC compiler")

# Qt 6.10.2 mingw_64 prefix
set(Qt6_DIR "D:/Qt/6.10.2/mingw_64/lib/cmake/Qt6" CACHE PATH "Qt6 CMake dir")
set(CMAKE_PREFIX_PATH "D:/Qt/6.10.2/mingw_64" CACHE PATH "Qt install prefix")

# Find programs from host only
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
