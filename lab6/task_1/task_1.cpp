#include "MemoryManager.h"
#include <iostream>

int main()
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    constexpr size_t blockSize = 16;
    constexpr size_t blockCount = 600 / (blockSize + sizeof(size_t)); 

    void* blocks[blockCount];
    for (size_t i = 0; i < blockCount; ++i)
    {
        blocks[i] = memoryManager.Allocate(blockSize);
        if (blocks[i] == nullptr)
        {
            std::cout << "null \n";
        }
    }

    return 0;
}
