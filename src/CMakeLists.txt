set(TARGET_NAME ${PROJECT_NAME})

find_package(QT NAMES Qt6 Qt5 CONFIG REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Widgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(BUILD_QMWIDGETS_SHARED_LIBS OR BUILD_SHARED_LIBS)
    add_library(${TARGET_NAME} SHARED)
    target_compile_definitions(${TARGET_NAME} PRIVATE QMWIDGETS_COMPILE_LIB)
else()
    add_library(${TARGET_NAME} STATIC)
    target_compile_definitions(${TARGET_NAME} PUBLIC QMWIDGETS_BUILD_STATIC)
endif()

target_link_libraries(${TARGET_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_precompile_headers(${TARGET_NAME} PUBLIC "stdafx.h")
target_include_directories(${TARGET_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_sources(${TARGET_NAME} PRIVATE
    qmassets.qrc
    qmanalogclock.h
    qmanalogclock.cpp
    qmcirclelight.h
    qmcirclelight.cpp
    qmgaugecar.h
    qmgaugecar.cpp
    qmimageanalogclock.h
    qmimageanalogclock.cpp
    qmlogindialog.h
    qmlogindialog.cpp
    qmlogindialog.ui
    qmswitch.h
    qmswitch.cpp
    qmthermometer.h
    qmthermometer.cpp
    qmcirclestatus.h
    qmcirclestatus.cpp
    qmframelesswindow.h
    qmframelesswindow.cpp
    qmcrossbutton.h
    qmcrossbutton.cpp
    qmgradientseparator.h
    qmgradientseparator.cpp
    qmautoreturnslider.h
    qmautoreturnslider.cpp
    qmimageslider.h
    qmimageslider.cpp
    qmninepatchpixmap.h
    qmninepatchpixmap.cpp
    qmdynamicmessage.h
    qmdynamicmessage.cpp
    qmtitlecontainer.h
    qmtitlecontainer.cpp
)

if(TAKEOVER_WIN32_HTCAPTION_EVENT)
    # set(TAKEOVER_WIN32_HTCAPTION_EVENT)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qmwidgets_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/qmwidgets_config.h @ONLY)

if(BUILD_QMWIDGETS_DESIGNER_PLUGIN)
    add_subdirectory(plugins)
endif()

if(BUILD_QMWIDGETS_QRCODEVIEW)
    target_sources(${TARGET_NAME} PRIVATE qmqrcodeview.cpp qmqrcodeview.h)
    target_link_libraries(${TARGET_NAME} PRIVATE qrencode)
endif()

if(BUILD_QMWIDGETS_HARDWARE_MONITOR)
    target_sources(${TARGET_NAME} PRIVATE qmhardwareinfo.h)

    if(WIN32)
        target_sources(${TARGET_NAME} PRIVATE qmhardwareinfo_win.cpp)
    endif()
endif()

if(QMWIDGET_INSTALL)
    install(TARGETS ${TARGET_NAME} DESTINATION bin)
endif()