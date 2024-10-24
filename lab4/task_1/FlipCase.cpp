#include "FlipCase.h"

FlipCase::FlipCase(const std::string& input, std::mutex& m) : inputFileName(input), outputFileName(input + ".out"), io_mutex(m) {}

char FlipCase::FlipCaseC(char newChar)
{
    unsigned char c = static_cast<unsigned char>(newChar);

    if (std::islower(c))
    {
        return std::toupper(c);
    }
    if (std::isupper(c))
    {
        return std::tolower(c);
    }
    return c;
}

void FlipCase::ProcessFile()
{
    try
    {
        FileHandler inputFile(inputFileName, std::ios::in);
        FileHandler outputFile(outputFileName, std::ios::out);

        char ch;
        while (inputFile.get().get(ch))
        {
            outputFile.get().put(FlipCaseC(ch));
        }

        std::lock_guard<std::mutex> lock(io_mutex);
        std::cout << "Processed: " << inputFileName << " into " << outputFileName << std::endl;

    }
    catch (const std::exception& e)
    {
        std::lock_guard<std::mutex> lock(io_mutex);
        std::cerr << "Error processing file " << inputFileName << ": " << e.what() << std::endl;
    }
}
