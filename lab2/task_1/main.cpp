#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <string>

std::string GetProcessName(DWORD processID) 
{
    std::string processName = "<unknown>";

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != nullptr) 
    {
        char lpBaseName[MAX_PATH];
        if (GetModuleBaseNameA(hProcess, nullptr, lpBaseName, sizeof(lpBaseName) / sizeof(char)))
        {
            processName = lpBaseName;
        }
        CloseHandle(hProcess);
    }

    return processName;
}

SIZE_T GetProcessMemoryUsage(DWORD processID) 
{
    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != nullptr) 
    {
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) 
        {
            CloseHandle(hProcess);
            return pmc.WorkingSetSize;
        }
        CloseHandle(hProcess);
    }
    return 0;
}

int main() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "Error: Unable to create toolhelp snapshot!" << std::endl;
        return 1;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) 
    {
        std::cerr << "Error: Unable to retrieve process information!" << std::endl;
        CloseHandle(hProcessSnap);
        return 1;
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

    CloseHandle(hProcessSnap);
    return 0;
}
