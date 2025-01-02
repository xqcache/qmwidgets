get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WLE)

find_package(QT NAMES Qt6 Qt5 CONFIG REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Widgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${TARGET_NAME}
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(${TARGET_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    qmwidgets
)
target_precompile_headers(${TARGET_NAME} PUBLIC "stdafx.h")