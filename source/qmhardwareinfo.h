#pragma once

#include "qmwidgets_global.h"
#include <cstdint>

class QmHardwareInfoPrivate;

class QMWIDGETS_EXPORT QmHardwareInfo {
public:
    QmHardwareInfo();
    ~QmHardwareInfo() noexcept;

    static qint64 totalMemorySize();
    static qint64 totalMemoryAvail();
    static qint64 totalMemoryUsed();

    static double totalMemoryUsage();
    double totalCpuUsage() const;

    static QStringList driveList();
    static qint64 driveUsedBytes(const QString& drive);
    static qint64 driveFreeBytes(const QString& drive);
    static qint64 driveTotalBytes(const QString& drive);
    static double driveUsage(const QString& drive);

private:
    QmHardwareInfoPrivate* d_ { nullptr };
};