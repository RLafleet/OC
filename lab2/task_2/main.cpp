#ifdef __linux__
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>
#include <unistd.h>

std::string GetProcessName(const std::string& pid) 
{
    std::string cmdlinePath = "/proc/" + pid + "/cmdline";
    std::ifstream cmdlineFile(cmdlinePath);
    std::string processName;
    if (cmdlineFile.is_open()) 
    {
        std::getline(cmdlineFile, processName, '\0');
        cmdlineFile.close();
    }
    if (processName.empty()) 
    {
        return "<unknown>";
    }
    return processName;
}

long GetProcessMemory(const std::string& pid) 
{
    std::string statusPath = "/proc/" + pid + "/status";
    std::ifstream statusFile(statusPath);
    std::string line;
    long memory = 0;

    if (statusFile.is_open()) 
    {
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
    }

    return memory; 
}

bool IsNumeric(const std::string& str) 
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
    DIR* procDir = opendir("/proc");
    if (procDir == nullptr) 
    {
        std::cerr << "Error: Unable to open /proc directory!" << std::endl;
        return 1;
    }

    struct dirent* entry;

    while ((entry = readdir(procDir)) != nullptr) 
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

    closedir(procDir);
    return 0;
}
#endif // __linux__
