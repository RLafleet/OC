#include <iostream>
#include <windows.h>
#include <versionhelpers.h>
#include <string>
#include <sstream>

typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

const int WINDOWS_VERSION_10 = 10;
const int WINDOWS_11_FIRST_BUILD_NUMBER = 22000;

std::string GetOsVersion()
{
    RTL_OSVERSIONINFOW osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(osInfo);

    // проверить на ошибку
    // попробовать без лоадера библиотеки "ntdll.dll"
    RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");
    std::ostringstream osVersion;

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
        osVersion << "OS: Windows 11";
    }
    else
    {
        osVersion << "OS: " << osInfo.dwMajorVersion;
    }

    if (osInfo.dwMinorVersion != 0)
    {
        osVersion << "." << osInfo.dwMinorVersion;
    }

    return osVersion.str();
}

std::string GetRAMInfo()
{
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    std::ostringstream ramInfo;

    if (!GlobalMemoryStatusEx(&memStatus))
    {
        return "Error: Unable to get memory information.";
    }

    DWORDLONG totalRAM = memStatus.ullTotalPhys / (1024 * 1024);
    DWORDLONG freeRAM = memStatus.ullAvailPhys / (1024 * 1024);
    ramInfo << "RAM: " << freeRAM << "MB / " << totalRAM << "MB";

    return ramInfo.str();
}

std::string GetProcessorInfo()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::ostringstream procInfo;
    procInfo << "Processors: " << sysInfo.dwNumberOfProcessors;

    return procInfo.str();
}

int main()
{
    std::cout << GetOsVersion() << std::endl;
    std::cout << GetRAMInfo() << std::endl;
    std::cout << GetProcessorInfo() << std::endl;

    return EXIT_SUCCESS;
}