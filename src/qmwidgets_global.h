#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
#if defined(QMWIDGETS_COMPILE_LIB)
#define QMWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define QMWIDGETS_EXPORT Q_DECL_IMPORT
#endif
#else
#define QMWIDGETS_EXPORT
#endif
