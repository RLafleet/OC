#ifdef __linux__
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include "CDirHandle.h"
#include <stdexcept>

static std::string GetProcessName(const std::string& pid)
{
    std::string cmdlinePath = "/proc/" + pid + "/cmdline";
    std::ifstream cmdlineFile(cmdlinePath);
    std::string processName;

    if (!cmdlineFile.is_open())
    {
        throw std::runtime_error("Failed to open cmdline file for PID: " + pid);
    }

    std::getline(cmdlineFile, processName, '\0');
    cmdlineFile.close();

    if (processName.empty())
    {
        return "<unknown>";
    }
    return processName;
}

static long GetProcessMemory(const std::string& pid)
{
    std::string statusPath = "/proc/" + pid + "/status";
    std::ifstream statusFile(statusPath);
    std::string line;
    long memory = 0;

    if (!statusFile.is_open())
    {
        throw std::runtime_error("Failed to open status file for PID: " + pid);
    }

    while (std::getline(statusFile, line))
    {
        if (line.substr(0, 6) == "VmRSS:")
        {
            std::istringstream iss(line);
            std::string label;
            iss >> label >> memory;
            break;
        }
    }

    statusFile.close();

    return memory;
}

static bool IsNumeric(const std::string& str)
{
    for (char const& c : str)
    {
        if (!std::isdigit(c))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    try
    {
        CDirHandle procDir(opendir("/proc"));
        if (!procDir.get())
        {
            throw std::runtime_error("Unable to open /proc directory!");
        }

        struct dirent* entry;

        while ((entry = readdir(procDir.get())) != nullptr)
        {
            if (IsNumeric(entry->d_name))
            {
                std::string pid = entry->d_name;
                std::string processName = GetProcessName(pid);
                long memoryUsage = GetProcessMemory(pid);

                std::cout << "PID = " << pid << std::endl;
                std::cout << "Process Name = " << processName << std::endl;
                std::cout << "Memory Usage (KB) = " << memoryUsage << " KB" << std::endl;
                std::cout << std::endl;
            }
        }

    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#endif // __linux__