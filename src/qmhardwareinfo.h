#pragma once

#include "qmwidgets_global.h"
#include <cstdint>

class QmHardwareInfoPrivate;

class QMWIDGETS_EXPORT QmHardwareInfo {
public:
    QmHardwareInfo();
    ~QmHardwareInfo() noexcept;

    std::int64_t totalMemorySize() const;
    std::int64_t totalMemoryAvail() const;
    std::int64_t totalMemoryUsed() const;

    double totalMemoryUsage() const;
    double totalCpuUsage() const;

private:
    QmHardwareInfoPrivate* d_ { nullptr };
};