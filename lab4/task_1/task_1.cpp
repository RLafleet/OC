#include "FlipCase.h"
// объяснить разницу между процессом и поток
int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cerr << "Usage: flip-case-mt <input files...>" << std::endl;
        return 1;
    }

    std::vector<std::jthread> jthreads;

    for (int i = 1; i < argc; ++i) 
    {
        // io mutex. Использовать osyncstream
        // чем отлтичается jthread
        jthreads.emplace_back([inputFile = std::string(argv[i])]
        {
            FlipCase flipCaseTask(inputFile);
            flipCaseTask.ProcessFile();
        });
    }

    // разобраться что произойдет если не будет этого

    //for (auto& thread : threads) 
    //{
        //thread.join();
    //}

    return 0;
}
