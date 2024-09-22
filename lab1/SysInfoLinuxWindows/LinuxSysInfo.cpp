#ifdef __linux__

#include "LinuxSysInfo.h"
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <thread>
#include <fstream>
#include <iostream>

std::string LinuxSysInfo::GetOSName() const
{
    return "Linux";
}

std::string LinuxSysInfo::GetOSVersion() const
{
    struct utsname os_info;
    if (uname(&os_info) != 0)
    {
        std::cerr << "Error: Unable to get OS version information." << std::endl;
        return "N/A";
    }
    return std::string(os_info.sysname) + " " + std::string(os_info.release);
}

uint64_t LinuxSysInfo::GetFreeMemory() const
{
    struct sysinfo mem_info;
    if (sysinfo(&mem_info) != 0) {
        std::cerr << "Error: Unable to get memory information." << std::endl;
        return 0;
    }
    return (mem_info.freeram * mem_info.mem_unit / (1024 * 1024));
}

uint64_t LinuxSysInfo::GetTotalMemory() const
{
    struct sysinfo mem_info;
    if (sysinfo(&mem_info) != 0) {
        std::cerr << "Error: Unable to get memory information." << std::endl;
        return 0;
    }
    return (mem_info.totalram * mem_info.mem_unit / (1024 * 1024));
}

unsigned LinuxSysInfo::GetProcessorCount() const
{
    return std::thread::hardware_concurrency();
}

#endif