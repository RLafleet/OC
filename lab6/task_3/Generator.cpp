#include "Generator.h"
#include <windows.h>
#include <thread>
#include <random>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <random>
#include "Utils.h"

static_assert(sizeof(Person) == 64, "Person 64 byte");

void GenerateFile(const std::string& fileName, size_t numRecords, size_t numThreads) 
{
    size_t fileSize = numRecords * sizeof(Person);

    HANDLE fileHandle = CreateFileA
    (
        fileName.c_str(),
        GENERIC_WRITE | GENERIC_READ,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (fileHandle == INVALID_HANDLE_VALUE) 
    {
        throw std::runtime_error("Failed to create file");
    }

    LARGE_INTEGER fileSizeLarge;
    fileSizeLarge.QuadPart = static_cast<LONGLONG>(fileSize);
    if (!SetFilePointerEx(fileHandle, fileSizeLarge, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) 
    {
        CloseHandle(fileHandle);
        throw std::runtime_error("Failed to set file size");
    }

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

    void* fileMem = MapViewOfFile(mappingHandle, FILE_MAP_WRITE, 0, 0, fileSize);
    
    if (!fileMem) 
    {
        CloseHandle(mappingHandle);
        CloseHandle(fileHandle);
        throw std::runtime_error("Failed to map view of file");
    }

    size_t chunkSize = numRecords / numThreads;
    std::vector<std::jthread> threads;

    for (size_t i = 0; i < numThreads; ++i) 
    {
        threads.emplace_back([i, chunkSize, fileMem, numRecords, numThreads]() {
            size_t start = i * chunkSize;
            size_t end = (i == numThreads - 1) ? numRecords : start + chunkSize;

            std::mt19937_64 rng(i);
            std::uniform_int_distribution<unsigned int> yearDist(1900, 2024);
            std::uniform_int_distribution<unsigned int> dayDist(1, 31);
            std::uniform_int_distribution<unsigned int> monthDist(1, 12);

            const std::vector<std::string> firstNames = { "John", "Jane", "Alice", "Bob" };
            const std::vector<std::string> lastNames = { "Smith", "Doe", "Brown", "Johnson" };

            Person* records = reinterpret_cast<Person*>(fileMem);

            for (size_t j = start; j < end; ++j) 
            {
                Person& p = records[j];
                p.birthDate.year = static_cast<uint16_t>(yearDist(rng));
                p.birthDate.day = static_cast<uint8_t>(dayDist(rng));
                p.birthDate.month = static_cast<uint8_t>(monthDist(rng));

                std::string fullName = firstNames[rng() % firstNames.size()] + " " +
                    lastNames[rng() % lastNames.size()];

                p.nameLength = static_cast<uint8_t>(min(fullName.size(), sizeof(p.name)));
                std::memcpy(p.name, fullName.data(), p.nameLength);
            }
        });
    }

    //RAII обертка и тоже мапить по частям

    UnmapViewOfFile(fileMem);
    CloseHandle(mappingHandle);
    CloseHandle(fileHandle);
}
