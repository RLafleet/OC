#ifndef ISYSINFO_H
#define ISYSINFO_H

#include <string>

class ISysInfo {
public:
    virtual ~ISysInfo() = default;

    virtual std::string GetOSName() const = 0;
    virtual std::string GetOSVersion() const = 0;
    virtual uint64_t GetFreeMemory() const = 0;
    virtual uint64_t GetTotalMemory() const = 0;
    virtual unsigned GetProcessorCount() const = 0;
};

#endif // ISYSINFO_H
