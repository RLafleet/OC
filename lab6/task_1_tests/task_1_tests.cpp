#define CATCH_CONFIG_MAIN
#include "../../../ood/Catch2/catch.hpp"

#include "../task_1/MemoryManager.h"

TEST_CASE("MemoryManager: Constructor and basic allocation")
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    SECTION("Constructor initializes memory manager")
    {
        REQUIRE_NOTHROW(MemoryManager(buffer, sizeof(buffer)));
    }

    SECTION("Allocate single block")
    {
        void* ptr = memoryManager.Allocate(100);
        REQUIRE(ptr != nullptr);
        REQUIRE(reinterpret_cast<uintptr_t>(ptr) % alignof(std::max_align_t) == 0);
    }

    SECTION("Allocate multiple blocks")
    {
        void* ptr1 = memoryManager.Allocate(100);
        REQUIRE(ptr1 != nullptr);

        void* ptr2 = memoryManager.Allocate(200);
        REQUIRE(ptr2 != nullptr);

        REQUIRE(ptr1 != ptr2); 
    }
}

TEST_CASE("MemoryManager: Alignment")
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    SECTION("Default alignment")
    {
        void* ptr = memoryManager.Allocate(50);
        REQUIRE(ptr != nullptr);
        REQUIRE(reinterpret_cast<uintptr_t>(ptr) % alignof(std::max_align_t) == 0);
    }

    SECTION("Custom alignment")
    {
        void* ptr = memoryManager.Allocate(50, 64);
        REQUIRE(ptr != nullptr);
        REQUIRE(reinterpret_cast<uintptr_t>(ptr) % 64 == 0);
    }

    SECTION("Invalid alignment")
    {
        void* ptr = memoryManager.Allocate(100, 3);
        REQUIRE(ptr == nullptr);
    }
}

TEST_CASE("MemoryManager: Edge cases")
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    SECTION("Zero size allocation")
    {
        void* ptr = memoryManager.Allocate(0);
        REQUIRE(ptr == nullptr); 
    }

    SECTION("Out of memory")
    {
        void* ptr1 = memoryManager.Allocate(1000); 
        REQUIRE(ptr1 != nullptr);

        void* ptr2 = memoryManager.Allocate(10)
        REQUIRE(ptr2 == nullptr);
    }
}

TEST_CASE("MemoryManager: Free")
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    SECTION("Free allocated block")
    {
        void* ptr = memoryManager.Allocate(100);
        REQUIRE(ptr != nullptr);

        REQUIRE_NOTHROW(memoryManager.Free(ptr));

        void* ptr2 = memoryManager.Allocate(100);
        REQUIRE(ptr2 == ptr);
    }

    SECTION("Free nullptr")
    {
        REQUIRE_NOTHROW(memoryManager.Free(nullptr));
    }

    SECTION("Free and reallocate")
    {
        void* ptr1 = memoryManager.Allocate(100);
        REQUIRE(ptr1 != nullptr);

        void* ptr2 = memoryManager.Allocate(200);
        REQUIRE(ptr2 != nullptr);

        memoryManager.Free(ptr1);

        void* ptr3 = memoryManager.Allocate(50);
        REQUIRE(ptr3 == ptr1); 
    }
}

TEST_CASE("MemoryManager: Stress test")
{
    alignas(std::max_align_t) char buffer[1024];
    MemoryManager memoryManager(buffer, sizeof(buffer));

    SECTION("Allocate and free many blocks")
    {
        constexpr size_t blockSize = 16;
        constexpr size_t blockCount = 512 / (blockSize + sizeof(size_t));

        void* blocks[blockCount];
        for (size_t i = 0; i < blockCount; ++i)
        {
            blocks[i] = memoryManager.Allocate(blockSize);
            REQUIRE(blocks[i] != nullptr);
        }

        for (size_t i = 0; i < blockCount; ++i)
        {
            memoryManager.Free(blocks[i]);
        }

        for (size_t i = 0; i < blockCount; ++i)
        {
            blocks[i] = memoryManager.Allocate(blockSize);
            REQUIRE(blocks[i] != nullptr);
        }
    }
}
