#pragma once

#include "qmwidgets_global.h"
#include <cstdint>

class QMWIDGETS_EXPORT QmHardwareInfo {
public:
    static std::int64_t totalMemorySize();
    static std::int64_t totalMemoryAvail();
    static std::int64_t totalMemoryUsed();
    static double totalCpuUsage();
};