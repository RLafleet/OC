#include <iostream>
#include <windows.h>
#include <versionhelpers.h>

typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

const int WINDOWS_VERSION_10 = 10;
const int WINDOWS_11_FIRST_BUILD_NUMBER = 22000;

void GetOsVersion()
{
    RTL_OSVERSIONINFOW osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(osInfo);

    RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");
    if (RtlGetVersion != nullptr) 
    {
        RtlGetVersion(&osInfo);
        if (osInfo.dwMajorVersion == WINDOWS_VERSION_10 
            && osInfo.dwBuildNumber >= WINDOWS_11_FIRST_BUILD_NUMBER) 
        {
            std::cout << "OS: Windows 11";
        }
        else
        {
            std::cout << "OS: " << osInfo.dwMajorVersion;
        }
        if (osInfo.dwMinorVersion != 0) {
            std::cout << "." << osInfo.dwMinorVersion;
        }
        std::cout << std::endl;

    }
    else {
        std::cout << "Unable to get OS version" << std::endl;
    }
}

void GetRAMInfo() 
{
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    if (GlobalMemoryStatusEx(&memStatus)) 
    {
        DWORDLONG totalRAM = memStatus.ullTotalPhys / (1024 * 1024);  
        DWORDLONG freeRAM = memStatus.ullAvailPhys / (1024 * 1024);

        std::cout << "RAM: " << freeRAM << "MB / " << totalRAM << "MB" << std::endl;
    }
    else {
        std::cout << "Unable to retrieve memory information" << std::endl;
    }
}

void GetProcessorInfo() 
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    std::cout << "Processors: " << sysInfo.dwNumberOfProcessors << std::endl;
}

int main() 
{
    GetOsVersion();
    GetRAMInfo();
    GetProcessorInfo();

    return 0;
}
