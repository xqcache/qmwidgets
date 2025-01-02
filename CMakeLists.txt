cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)

project(qmwidgets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "" ON)
option(BUILD_TESTS "" OFF)
option(BUILD_DESIGNER_PLUGIN "" ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
