#include "Generator.h"
#include "Sorter.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) 
{
    if (argc < 4) 
    {
        std::cerr << "Usage:\n";
        std::cerr << "  sorter generate FILE_NAME NUM_RECORDS NUM_THREADS\n";
        std::cerr << "  sorter sort FILE_NAME NUM_RECORDS NUM_THREADS\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string fileName = argv[2];
    size_t numThreads = std::stoul(argv[argc - 1]);

    if (mode == "generate") 
    {
        size_t numRecords = std::stoul(argv[3]);
        GenerateFile(fileName, numRecords, numThreads);
    }
    else if (mode == "sort") 
    {
        size_t numRecords = std::stoul(argv[3]);; 
        SortFile(fileName, numRecords, numThreads);
    }
    else 
    {
        std::cerr << "Unknown mode: " << mode << "\n";
        return 1;
    }

    return 0;
}
