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

inline void __qmwidgets_resource_lazy_load()
{
    static struct QmWidgetResourceInitializer {
        QmWidgetResourceInitializer()
        {
            qInitResources_qmassets();
        }
        ~QmWidgetResourceInitializer()
        {
            qCleanupResources_qmassets();
        }
    } __qmwidgets_resource_initializer;
}
#define QMWIDGETS_INITIALIZE_RESOURCE __qmwidgets_resource_lazy_load();
#endif
