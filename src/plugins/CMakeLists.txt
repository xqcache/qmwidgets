cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(TARGET_NAME ${PROJECT_NAME}plugin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Designer)

add_library(${TARGET_NAME} SHARED
    qmwidgets.h
    qmwidgets.cpp
    qmanalogclockplugin.h
    qmanalogclockplugin.cpp
    qmcirclelightplugin.h
    qmcirclelightplugin.cpp
    qmgaugecarplugin.h
    qmgaugecarplugin.cpp
    qmimageanalogclockplugin.h
    qmimageanalogclockplugin.cpp
    qmswitchplugin.h
    qmswitchplugin.cpp
    qmthermometerplugin.h
    qmthermometerplugin.cpp
    qmimagesliderplugin.h
    qmimagesliderplugin.cpp
)
target_link_libraries(${TARGET_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Designer
    qmwidgets
)
set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")

# target_precompile_headers(${TARGET_NAME} PRIVATE "stdafx.h")
target_compile_definitions(${TARGET_NAME} PRIVATE BUILD_STATIC)
