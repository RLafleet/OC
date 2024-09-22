#ifndef SYS_INFO_FACTORY_H
#define SYS_INFO_FACTORY_H

#include "ISysInfo.h"
#include <memory>

class SysInfoFactory {
public:
    static std::unique_ptr<ISysInfo> CreateSysInfo();
};

#endif // SYS_INFO_FACTORY_H
