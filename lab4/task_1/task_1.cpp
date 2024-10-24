#include "FlipCase.h"

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cerr << "Usage: flip-case-mt <input files...>" << std::endl;
        return 1;
    }

    std::mutex io_mutex;
    std::vector<std::thread> threads;

    for (int i = 1; i < argc; ++i) 
    {
        threads.emplace_back([&io_mutex, inputFile = std::string(argv[i])] 
            {
            FlipCase flipCaseTask(inputFile, io_mutex);
            flipCaseTask.ProcessFile();
            });
    }

    for (auto& thread : threads) 
    {
        thread.join();
    }

    return 0;
}
