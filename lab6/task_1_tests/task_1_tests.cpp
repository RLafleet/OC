#include <thread>
#include <vector>
#include <iostream>

void TestBasicFunctionality()
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    auto ptr1 = memoryManager.Allocate(100);
    assert(ptr1 != nullptr);

    auto ptr2 = memoryManager.Allocate(200, 16);
    assert(ptr2 != nullptr);
    assert(reinterpret_cast<uintptr_t>(ptr2) % 16 == 0);

    memoryManager.Free(ptr1);
    memoryManager.Free(ptr2);
}

void TestAlignment()
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    auto ptr = memoryManager.Allocate(50, 32);
    assert(ptr != nullptr);
    assert(reinterpret_cast<uintptr_t>(ptr) % 32 == 0);
}

void TestOutOfMemory()
{
    alignas(std::max_align_t) char buffer[256];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    auto ptr1 = memoryManager.Allocate(200);
    assert(ptr1 != nullptr);

    auto ptr2 = memoryManager.Allocate(100);
    assert(ptr2 == nullptr); // Недостаточно памяти
}

void TestMultithreading()
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    auto AllocateInThread = [&memoryManager]() {
        for (int i = 0; i < 100; ++i)
        {
            auto ptr = memoryManager.Allocate(10);
            assert(ptr != nullptr);
            memoryManager.Free(ptr);
        }
        };

    std::thread t1(AllocateInThread);
    std::thread t2(AllocateInThread);

    t1.join();
    t2.join();
}

int main()
{
    TestBasicFunctionality();
    TestAlignment();
    TestOutOfMemory();
    TestMultithreading();

    std::cout << "All tests passed!\n";
}
