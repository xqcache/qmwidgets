#include "qmhardwareinfo.h"
#include <mutex>

#pragma comment(lib, "pdh.lib")
// clang-format off
#include <Windows.h>
#include <Pdh.h>
// clang-format on

namespace {
std::mutex g_mutex;
PDH_HQUERY g_cpu_query = nullptr;
PDH_HCOUNTER g_cpu_counter = nullptr;
} // namespace

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

std::int64_t QmHardwareInfo::totalMemoryUsed()
{
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPhys - mem_info.ullAvailPhys;
}

double QmHardwareInfo::totalCpuUsage()
{
    std::unique_lock<std::mutex> lock(g_mutex);
    if (!g_cpu_query) {
        PdhOpenQuery(NULL, NULL, &g_cpu_query);
        // You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
        PdhAddEnglishCounter(g_cpu_query, L"\\Processor(_Total)\\% Processor Time", NULL, &g_cpu_counter);
        PdhCollectQueryData(g_cpu_query);

        // TODO: 需要关闭查询释放资源，可能不能使用静态类
    }
    PDH_FMT_COUNTERVALUE value;
    PdhCollectQueryData(g_cpu_query);
    PdhGetFormattedCounterValue(g_cpu_counter, PDH_FMT_DOUBLE, NULL, &value);
    return value.doubleValue;
}