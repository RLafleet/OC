#include <iostream>
#include "SysInfo.h"

int main() {
    SysInfo sysInfo;

    std::cout << "Operating system: " << sysInfo.GetOSName() << std::endl;
    std::cout << "Version OC: " << sysInfo.GetOSVersion() << std::endl;
    std::cout << "Amount of logical process: " << sysInfo.GetProcessorCount() << std::endl;
    std::cout << "Free memory: " << sysInfo.GetFreeMemory() / (1024 * 1024) << " MB" << std::endl;
    std::cout << "All of memory: " << sysInfo.GetTotalMemory() / (1024 * 1024) << " MB" << std::endl;

    return 0;
}