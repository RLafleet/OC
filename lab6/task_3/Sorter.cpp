#include "Sorter.h"
#include <windows.h>
#include <algorithm>
#include <string_view>
#include <stdexcept>
#include <string>
#include "Utils.h"

static_assert(sizeof(Person) == 64, "Person 64 byte");

void SortFile(const std::string& fileName, size_t numRecords, size_t numThreads) 
{
    size_t fileSize = numRecords * sizeof(Person);

    HANDLE fileHandle = CreateFileA
    (
        fileName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (fileHandle == INVALID_HANDLE_VALUE) 
    {
        throw std::runtime_error("Failed to open file");
    }

    // RAII обертки

    HANDLE mappingHandle = CreateFileMappingA
    (
        fileHandle,
        nullptr,
        PAGE_READWRITE,
        static_cast<DWORD>(fileSize >> 32),
        static_cast<DWORD>(fileSize & 0xFFFFFFFF),
        nullptr
    );

    if (!mappingHandle) 
    {
        CloseHandle(fileHandle);
        throw std::runtime_error("Failed to create file mapping");
    }

    // нельзя map для всего. Сделать кусками, константы кратные размеру страниц
    void* fileMem = MapViewOfFile
    (
        mappingHandle, 
        FILE_MAP_WRITE, 
        0, 
        0, 
        fileSize
    );

    if (!fileMem) 
    {
        CloseHandle(mappingHandle);
        CloseHandle(fileHandle);
        throw std::runtime_error("Failed to map view of file");
    }

    Person* records = reinterpret_cast<Person*>(fileMem);

    // аккуратнее сортировать кусками
    std::sort(records, records + numRecords, [](const Person& a, const Person& b)
        {
            // std::tie
            if (a.birthDate.year != b.birthDate.year) 
            {
                return a.birthDate.year < b.birthDate.year;
            }

            if (a.birthDate.month != b.birthDate.month) 
            {
                return a.birthDate.month < b.birthDate.month;
            }

            if (a.birthDate.day != b.birthDate.day) 
            {
                return a.birthDate.day < b.birthDate.day;
            }

            return std::string_view(a.name, a.nameLength) < std::string_view(b.name, b.nameLength);
        });

    UnmapViewOfFile(fileMem);
    CloseHandle(mappingHandle);
    CloseHandle(fileHandle);
}
