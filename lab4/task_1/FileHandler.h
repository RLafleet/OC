#ifndef FILE_HANDLER
#define FILE_HANDLER

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <stdexcept>
#include <mutex>
#include <filesystem>

class FileHandler
{
    std::fstream file;
public:
    FileHandler(const std::string& filename, std::ios::openmode mode);
    std::fstream& get();
    ~FileHandler();
};

#endif //FILE_HANDLER