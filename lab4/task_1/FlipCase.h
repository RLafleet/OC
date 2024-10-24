#ifndef FLIP_CASE_H
#define FLIP_CASE_H

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <stdexcept>
#include <mutex>
#include <filesystem>
#include "FileHandler.h"

class FlipCase
{
public:
    FlipCase(const std::string& input, std::mutex& m);
    void ProcessFile();

private:
    std::string inputFileName;
    std::string outputFileName;
    std::mutex& io_mutex;

    char FlipCaseC(char newChar);
};

#endif // FLIP_CASE_H