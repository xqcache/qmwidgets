set(TARGET_NAME ${PROJECT_NAME})

find_package(QT NAMES Qt6 Qt5 CONFIG REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Widgets Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(BUILD_QMWIDGETS_SHARED_LIBS OR BUILD_SHARED_LIBS)
    add_library(${TARGET_NAME} SHARED)
    target_compile_definitions(${TARGET_NAME} PRIVATE QMWIDGETS_COMPILE_LIB)
else()
    add_library(${TARGET_NAME} STATIC)
    target_compile_definitions(${TARGET_NAME} PUBLIC QMWIDGETS_BUILD_STATIC)
endif()

target_link_libraries(${TARGET_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_precompile_headers(${TARGET_NAME} PRIVATE "stdafx.h")
target_include_directories(${TARGET_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_include_directories(${TARGET_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

set(PRIVATE_SOURCES
    qmassets.qrc
    qmwidgets_global.h
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
    qmframelessdialog.h
    qmframelessdialog.cpp
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
    qmblinklabel.h
    qmblinklabel.cpp
    qmshadowhelperwidget.h
    qmshadowhelperwidget.cpp
    qmcollapsiblewidget.h
    qmcollapsiblewidget.cpp
    qmimagebutton.h
    qmimagebutton.cpp
    qmimageshadowwidget.h
    qmimageshadowwidget.cpp
    qmasyncblockerwidget.h
    qmasyncblockerwidget.cpp
    qmshadowframe.h
    qmshadowframe.cpp
)

target_sources(${TARGET_NAME} PRIVATE ${PRIVATE_SOURCES})
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "d")

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
    get_target_property(_header_files ${TARGET_NAME} SOURCES)
    list(FILTER _header_files INCLUDE REGEX ".*\\.h$")
    list(TRANSFORM _header_files PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

    install(TARGETS ${TARGET_NAME} EXPORT QmWidgetsTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT QmWidgets
    )

    if(BUILD_QMWIDGETS_SHARED_LIBS OR BUILD_SHARED_LIBS)
        install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug)
    endif()

    install(FILES ${_header_files} ${CMAKE_CURRENT_BINARY_DIR}/qmwidgets_config.h
        TYPE INCLUDE
        COMPONENT QmWidgets
    )

    install(EXPORT QmWidgetsTargets
        FILE ${TARGET_NAME}Targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}
        COMPONENT QmWidgets
    )

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/${TARGET_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}Config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}"
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}Config.cmake
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_NAME}"
        COMPONENT QmWidgets
    )

    set(CPACK_PACKAGE_NAME ${TARGET_NAME})
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_VENDOR xqliang)
    set(CPACK_PACKAGE_DESCRIPTION "Qt自定义控件")
    set(CPACK_GENERATOR ZIP)

    include(CPack)
endif()