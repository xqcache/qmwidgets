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

int64_t QmHardwareInfo::totalMemorySize() const
{
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPhys;
}

int64_t QmHardwareInfo::totalMemoryAvail() const
{
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullAvailPhys;
}

std::int64_t QmHardwareInfo::totalMemoryUsed() const
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