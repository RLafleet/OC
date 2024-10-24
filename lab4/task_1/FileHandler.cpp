#include "FileHandler.h"

FileHandler::FileHandler(const std::string& filename, std::ios::openmode mode)
{
    file.open(filename, mode);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}

std::fstream& FileHandler::get()
{
    return file;
}

FileHandler::~FileHandler()
{
    if (file.is_open())
    {
        file.close();
    }
}