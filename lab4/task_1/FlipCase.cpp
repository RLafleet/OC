#include "FlipCase.h"

FlipCase::FlipCase(const std::string& input)
    : inputFileName(input), outputFileName(input + ".out") {}

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

        std::osyncstream(std::cout) << "Processed: " << inputFileName
            << " into " << outputFileName << std::endl;
    }
    catch (const std::exception& e)
    {
        std::osyncstream(std::cerr) << "Error processing file "
            << inputFileName << ": " << e.what() << std::endl;
    }
}
