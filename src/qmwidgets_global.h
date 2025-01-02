#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
#define QMWIDGETS_INITIALIZE_RESOURCE
#if defined(QMWIDGETS_COMPILE_LIB)
#define QMWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define QMWIDGETS_EXPORT Q_DECL_IMPORT
#endif
#else
#define QMWIDGETS_EXPORT

extern int qInitResources_qmassets();
extern int qCleanupResources_qmassets();

struct QmWidgetResourceInitializer {
    QmWidgetResourceInitializer()
    {
        qInitResources_qmassets();
    }
    ~QmWidgetResourceInitializer()
    {
        qCleanupResources_qmassets();
    }
};

#define QMWIDGETS_INITIALIZE_RESOURCE \
    QmWidgetResourceInitializer __g_qmwidgets_resource_initializer__;
#endif
