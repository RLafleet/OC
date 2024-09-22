#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iostream>

void GetOCVersion()
{
    struct utsname os_info;
    if (uname(&os_info) != 0)
    {
        std::cerr << "Error: Unable to get OS version information." << std::endl;
        return;
    }

    std::cout << "OS: " << os_info.sysname << " " << os_info.release << std::endl;
}

void GetRAMInfo()
{
    struct sysinfo mem_info;
    if (sysinfo(&mem_info) != 0)
    {
        std::cerr << "Error: Unable to get memory information." << std::endl;
        return;
    }

    long long total_ram = mem_info.totalram * mem_info.mem_unit;
    long long free_ram = mem_info.freeram * mem_info.mem_unit;

    std::cout << "RAM: " << free_ram / 1048576 << "MB / " << total_ram / 1048576 << "MB" << std::endl;
}

void GetProcInfo()
{
    long num_processors = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_processors < 0)
    {
        std::cerr << "Error: Unable to get the number of processors." << std::endl;
        return;
    }

    std::cout << "Processors: " << num_processors << std::endl;
}

int main()
{
    GetOCVersion();
    GetRAMInfo();
    GetProcInfo();
    return EXIT_SUCCESS;
}