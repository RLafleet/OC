#ifndef LINUX_SYS_INFO_H
#define LINUX_SYS_INFO_H

#include "ISysInfo.h"
#include <string>
#include <cstdint>

class LinuxSysInfo : public ISysInfo {
public:
    std::string GetOSName() const override;

    std::string GetOSVersion() const override;

    uint64_t GetFreeMemory() const override;

    uint64_t GetTotalMemory() const override;

    unsigned GetProcessorCount() const override;
};

#endif // LINUX_SYS_INFO_H