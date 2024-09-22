#ifdef _WIN32

#include "WindowsSysInfo.h"
#include <windows.h>
#include <winternl.h>
#include <thread>
#include <iostream>

#pragma comment(lib, "ntdll.lib")

const int WINDOWS_11_FIRST_BUILD_NUMBER = 22000;
const int WINDOWS_VERSION_10 = 10;


std::string WindowsSysInfo::GetOSName() const 
{
    return "Windows";
}

std::string WindowsSysInfo::GetOSVersion() const 
{
    RTL_OSVERSIONINFOW osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(osInfo);

    using RtlGetVersionPtr = NTSTATUS(WINAPI*)(PRTL_OSVERSIONINFOW);
    RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");

    if (RtlGetVersion == nullptr) 
    {
        return "Error: Unable to find RtlGetVersion function.";
    }

    LONG result = RtlGetVersion(&osInfo);
    if (result != 0) 
    {
        return "Error: Unable to get OS version.";
    }

    if (osInfo.dwMajorVersion == WINDOWS_VERSION_10 && osInfo.dwBuildNumber >= WINDOWS_11_FIRST_BUILD_NUMBER)
    {
        return "11";
    }

    return std::to_string(osInfo.dwMajorVersion) + "." + std::to_string(osInfo.dwMinorVersion);
}

uint64_t WindowsSysInfo::GetFreeMemory() const 
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memInfo)) 
    {
        std::cerr << "Error: Unable to get memory information." << std::endl;
        return 0;
    }
    return (memInfo.ullAvailPhys / (1024 * 1024));
}

uint64_t WindowsSysInfo::GetTotalMemory() const {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memInfo)) {
        std::cerr << "Error: Unable to get memory information." << std::endl;
        return 0;
    }
    return (memInfo.ullTotalPhys / (1024 * 1024));
}

unsigned WindowsSysInfo::GetProcessorCount() const {
    return std::thread::hardware_concurrency();
}

#endif