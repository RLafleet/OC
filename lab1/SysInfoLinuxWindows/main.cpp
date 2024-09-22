#include "SysInfoFactory.h"
#include <iostream>

int main() {
    auto sysInfo = SysInfoFactory::CreateSysInfo();

    if (sysInfo) {
        std::cout << "OS Name: " << sysInfo->GetOSName() << std::endl;
        std::cout << "OS Version: " << sysInfo->GetOSVersion() << std::endl;
        std::cout << "Free Memory: " << sysInfo->GetFreeMemory() << " MB" << std::endl;
        std::cout << "Total Memory: " << sysInfo->GetTotalMemory() << " MB" << std::endl;
        std::cout << "Processor Count: " << sysInfo->GetProcessorCount() << std::endl;
    }
    else {
        std::cerr << "Error: Unsupported platform." << std::endl;
    }

    return 0;
}
