#include <iostream>
#include <string>
#include <Windows.h>

int main() {
    // Informations sur le système d'exploitation
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);

    std::cout << "OS: Windows " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << std::endl;

    // Informations sur la mémoire
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    long long totalPhys = memInfo.ullTotalPhys;
    long long freePhys = memInfo.ullAvailPhys;

    std::cout << "RAM: " << freePhys / 1048576 << "MB / " << totalPhys / 1048576 << "MB" << std::endl;

    // Nombre de processeurs
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    std::cout << "Processors: " << sysinfo.dwNumberOfProcessors << std::endl;

    return 0;
}