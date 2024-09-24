#ifdef __linux__

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cctype>

void FlipCaseAndWrite(const std::string& inputFile) 
{
    std::string outputFile = inputFile + ".out";

    std::ifstream inFile(inputFile);
    if (!inFile) 
    {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        exit(1);
    }

    std::ofstream outFile(outputFile);
    if (!outFile) 
    { 
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        exit(1);
    }

    char ch;
    while (inFile.get(ch)) 
    {
        if (std::isupper(ch)) 
        {
            outFile.put(std::tolower(ch));
        }
        else if (std::islower(ch)) 
        {
            outFile.put(std::toupper(ch));
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
            std::cout << "Process " << getpid() << " is processing " << argv[i] << std::endl;
            FlipCaseAndWrite(argv[i]);
            exit(0);
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