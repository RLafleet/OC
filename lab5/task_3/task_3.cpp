
#include "ThreadPool.h"

#include <iostream>

int main()
{
    ThreadPool pool(4);
    std::atomic<int> counter = 0;

    pool.Dispatch([&] { throw std::runtime_error("Test exception"); });
    pool.Dispatch([&] { ++counter; });
    pool.Wait();


    std::cout << counter << " Hello World!\n";
}
