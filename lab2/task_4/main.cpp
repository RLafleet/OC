#ifdef __linux__

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cctype>
#include <stdexcept>

void FlipCaseAndWrite(const std::string& inputFile)
{
    std::string outputFile = inputFile + ".out";

    std::ifstream inFile(inputFile);
    if (!inFile)
    {
        throw std::runtime_error("Error opening input file: " + inputFile);
    }

    std::ofstream outFile(outputFile);
    if (!outFile)
    {
        throw std::runtime_error("Error opening output file: " + outputFile);
    }

    // нельзя просто так вызывать с char
    char ch;
    while (inFile.get(ch))
    {
        wunsigned char uc = static_cast<unsigned char>(ch);

        if (std::isupper(uc)) 
        {
            outFile.put(std::tolower(uc));
        }
        else if (std::islower(uc)) 
        {
            outFile.put(std::toupper(uc));
        }
        else 
        {
            outFile.put(ch); 
        }
    }

    inFile.close();
    outFile.close();

    std::cout << "Process " << getpid() << " has finished writing to " << outputFile << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file1> <input_file2> ..." << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            std::cerr << "Fork failed!" << std::endl;
            return 1;
        }
        else if (pid == 0)
        {
            try
            {
                std::cout << "Process " << getpid() << " is processing " << argv[i] << std::endl;
                FlipCaseAndWrite(argv[i]);
                // не использовать exit
                return 0;
            }
            catch (const std::runtime_error& e)
            {
                std::cerr << "Error in child process " << getpid() << ": " << e.what() << std::endl;
                return 1;
            }
        }
    }

    while (wait(NULL) > 0);

    for (int i = 1; i < argc; ++i)
    {
        std::cout << "Child process has finished for " << argv[i] << std::endl;
    }

    return 0;
}

#endif //__linux__