#include "qmhardwareinfo.h"
#include <mutex>

#pragma comment(lib, "pdh.lib")
// clang-format off
#include <Windows.h>
#include <Pdh.h>
// clang-format on

struct QmHardwareInfoPrivate {
    std::mutex mutex;
    PDH_HQUERY query = nullptr;
    PDH_HCOUNTER cpu_counter = nullptr;
};

QmHardwareInfo::QmHardwareInfo()
    : d_(new QmHardwareInfoPrivate)
{
}

QmHardwareInfo::~QmHardwareInfo() noexcept
{
    if (d_->query) {
        PdhCloseQuery(d_->query);
    }
    delete d_;
}

int64_t QmHardwareInfo::totalMemorySize()
{
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPhys;
}

int64_t QmHardwareInfo::totalMemoryAvail()
{
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullAvailPhys;
}

qint64 QmHardwareInfo::totalMemoryUsed()
{
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPhys - mem_info.ullAvailPhys;
}

double QmHardwareInfo::totalMemoryUsage() const
{
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);

    return 100.0 * (mem_info.ullTotalPhys - mem_info.ullAvailPhys) / mem_info.ullTotalPhys;
}

double QmHardwareInfo::totalCpuUsage() const
{
    std::unique_lock<std::mutex> lock(d_->mutex);

    if (!d_->query) {
        PdhOpenQuery(NULL, NULL, &d_->query);
    }
    if (!d_->cpu_counter) {
        PdhAddEnglishCounter(d_->query, L"\\Processor(_Total)\\% Processor Time", NULL, &(d_->cpu_counter));
        PdhCollectQueryData(d_->query);
    }
    PDH_FMT_COUNTERVALUE value;
    PdhCollectQueryData(d_->query);
    PdhGetFormattedCounterValue(d_->cpu_counter, PDH_FMT_DOUBLE, NULL, &value);
    return value.doubleValue;
}

QStringList QmHardwareInfo::driveList()
{
    QStringList result;
    DWORD drives = GetLogicalDrives();
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        if (drives & (1 << (ch - 'A'))) {
            result.append(QString("%1:").arg(ch));
        }
    }
    return result;
}

qint64 QmHardwareInfo::driveUsedBytes(const QString& drive)
{
    // 当前用户剩余可以用的字节数
    [[maybe_unused]] ULARGE_INTEGER user_free_avaliable;
    // 当前用户总共可用的字节数
    ULARGE_INTEGER user_total_bytes;
    // 磁盘总共可用字节数
    ULARGE_INTEGER total_free_bytes;
    if (GetDiskFreeSpaceEx(drive.toStdWString().c_str(), &user_free_avaliable, &user_total_bytes, &total_free_bytes)) {
        return user_total_bytes.QuadPart - user_free_avaliable.QuadPart;
    }
    return 0;
}

qint64 QmHardwareInfo::driveFreeBytes(const QString& drive)
{
    // 当前用户剩余可以用的字节数
    [[maybe_unused]] ULARGE_INTEGER user_free_avaliable;
    // 当前用户总共可用的字节数
    ULARGE_INTEGER user_total_bytes;
    // 磁盘总共可用字节数
    ULARGE_INTEGER total_free_bytes;
    if (GetDiskFreeSpaceEx(drive.toStdWString().c_str(), &user_free_avaliable, &user_total_bytes, &total_free_bytes)) {
        return user_free_avaliable.QuadPart;
    }
    return 0;
}

qint64 QmHardwareInfo::driveTotalBytes(const QString& drive)
{
    // 当前用户剩余可以用的字节数
    [[maybe_unused]] ULARGE_INTEGER user_free_avaliable;
    // 当前用户总共可用的字节数
    ULARGE_INTEGER user_total_bytes;
    // 磁盘总共可用字节数
    ULARGE_INTEGER total_free_bytes;
    if (GetDiskFreeSpaceEx(drive.toStdWString().c_str(), &user_free_avaliable, &user_total_bytes, &total_free_bytes)) {
        return user_total_bytes.QuadPart;
    }
    return 0;
}

double QmHardwareInfo::driveUsage(const QString& drive)
{
    // 当前用户剩余可以用的字节数
    [[maybe_unused]] ULARGE_INTEGER user_free_avaliable;
    // 当前用户总共可用的字节数
    ULARGE_INTEGER user_total_bytes;
    // 磁盘总共可用字节数
    ULARGE_INTEGER total_free_bytes;
    if (GetDiskFreeSpaceEx(drive.toStdWString().c_str(), &user_free_avaliable, &user_total_bytes, &total_free_bytes)) {
        if (user_total_bytes.QuadPart == 0) {
            return 0;
        }
        return 100.0 * (user_total_bytes.QuadPart - user_free_avaliable.QuadPart) / static_cast<double>(user_total_bytes.QuadPart);
    }
    return 0;
}