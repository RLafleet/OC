#include "SysInfoFactory.h"
#ifdef _WIN32
#include "WindowsSysInfo.h"
#endif

#ifdef __linux__
#include "LinuxSysInfo.h"
#endif

std::unique_ptr<ISysInfo> SysInfoFactory::CreateSysInfo() {
#ifdef _WIN32
    return std::make_unique<WindowsSysInfo>();
#elif __linux__
    return std::make_unique<LinuxSysInfo>();
#else
    return nullptr;
#endif
}