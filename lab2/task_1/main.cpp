#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <string>
#include <atlbase.h>
#include <stdexcept>

std::string GetProcessName(DWORD processID)
{
    std::string processName = "<unknown>";
    CHandle hProcess(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID));

    if (hProcess)
    {
        char lpBaseName[MAX_PATH];
        if (GetModuleBaseNameA(hProcess, nullptr, lpBaseName, sizeof(lpBaseName)))
        {
            processName = lpBaseName;
        }
    }
    else
    {
        throw std::runtime_error("Failed to open process for ID: " + std::to_string(processID));
    }

    return processName;
}

SIZE_T GetProcessMemoryUsage(DWORD processID)
{
    PROCESS_MEMORY_COUNTERS pmc;
    CHandle hProcess(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID));

    if (hProcess && GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
    {
        return pmc.WorkingSetSize;
    }

    throw std::runtime_error("Failed to get memory info for process ID: " + std::to_string(processID));
}

int main() {
    try {
        CHandle hProcessSnap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

        if (hProcessSnap == INVALID_HANDLE_VALUE)
        {
            throw std::runtime_error("Unable to create toolhelp snapshot!");
        }

        PROCESSENTRY32 pe32{ .dwSize = sizeof(PROCESSENTRY32) };

        if (!Process32First(hProcessSnap, &pe32))
        {
            throw std::runtime_error("Unable to retrieve process information!");
        }

        do
        {
            std::string processName = GetProcessName(pe32.th32ProcessID);
            SIZE_T memoryUsage = GetProcessMemoryUsage(pe32.th32ProcessID) / 1024;

            std::cout << "PID = " << pe32.th32ProcessID << std::endl;
            std::cout << "Process Name = " << processName << std::endl;
            std::cout << "Memory Usage (KB) = " << memoryUsage << " KB" << std::endl;
            std::cout << std::endl;

        } while (Process32Next(hProcessSnap, &pe32));

    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}