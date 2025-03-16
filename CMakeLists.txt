cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)

project(qmwidgets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "" ON)
option(BUILD_QMWIDGETS_TESTS "" OFF)
option(BUILD_QMWIDGETS_DESIGNER_PLUGIN "" OFF)
option(BUILD_QMWIDGETS_QRCODEVIEW "" OFF)

option(BUILD_QMWIDGETS_HARDWARE_MONITOR "" OFF)

option(TAKEOVER_WIN32_HTCAPTION_EVENT "在QmFramelessWindow中接管Win32 HTCAPTION事件" ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

if(BUILD_QMWIDGETS_TESTS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    add_subdirectory(tests)
endif()

add_subdirectory(src)
add_subdirectory(3rdparty)