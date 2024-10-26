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
public:
    FileHandler(const std::string& filename, std::ios::openmode mode);
    std::fstream& get();
    ~FileHandler();

private:
    std::fstream file;
};

#endif //FILE_HANDLER