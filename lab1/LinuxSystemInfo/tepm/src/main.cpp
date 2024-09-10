#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iostream>

void GetOCVersion()
{
    struct utsname os_info;
    uname(&os_info);
    std::cout << "OS: " << os_info.sysname << " " << os_info.release << std::endl;
}

void GetRAMInfo()
{
    struct sysinfo mem_info;
    sysinfo(&mem_info);
    long long total_ram = mem_info.totalram;
    total_ram *= mem_info.mem_unit;
    long long free_ram = mem_info.freeram;
    free_ram *= mem_info.mem_unit;
    std::cout << "RAM: " << free_ram / 1048576 << "MB / " << total_ram / 1048576 << "MB" << std::endl;
}

void GetProcInfo()
{
    long num_processors = sysconf(_SC_NPROCESSORS_ONLN);
    std::cout << "Processors: " << num_processors << std::endl;
}

int main()
{
    GetOCVersion();
    GetRAMInfo();
    GetProcInfo();
    return EXIT_SUCCESS;
}
