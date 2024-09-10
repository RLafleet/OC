#include <string>
#include <thread>
#include <fstream>
#include <iostream>
#include <windows.h>

#ifdef _WIN32
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")
#endif

class SysInfo {
public:
    std::string GetOSName() const {
#ifdef _WIN32
        return "Windows";
#else
        return "Linux";
#endif
    }

    std::string GetOSVersion() const {
#ifdef _WIN32
        RTL_OSVERSIONINFOW osInfo;
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);

        using RtlGetVersionPtr = NTSTATUS(WINAPI*)(PRTL_OSVERSIONINFOW);
        RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");

        if (RtlGetVersion != nullptr) {
            RtlGetVersion(&osInfo);
            if (osInfo.dwMajorVersion == 10 && osInfo.dwBuildNumber >= 22000) {
                return "Windows 11";
            }
            else if (osInfo.dwMajorVersion == 10) {
                return "Windows 10";
            }
            else {
                return std::to_string(osInfo.dwMajorVersion) + "." + std::to_string(osInfo.dwMinorVersion);
            }
        }
        else {
            return "Unable to get OS version";
        }
#else
        std::ifstream versionFile("/proc/version");
        std::string version;
        std::getline(versionFile, version);
        return version;
#endif
    }

    uint64_t GetFreeMemory() const {
#ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        return memInfo.ullAvailPhys;
#else
        struct sysinfo memInfo;
        sysinfo(&memInfo);
        return memInfo.freeram;
#endif
    }

    uint64_t GetTotalMemory() const {
#ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        return memInfo.ullTotalPhys;
#else
        struct sysinfo memInfo;
        sysinfo(&memInfo);
        return memInfo.totalram;
#endif
    }

    unsigned GetProcessorCount() const {
        return std::thread::hardware_concurrency();
    }
};