#define CATCH_CONFIG_MAIN
#include "../../../ood/Catch2/catch.hpp"

#include "../task_3/ThreadPool.h"
#include <atomic>
#include <thread>
#include <chrono>

TEST_CASE("Dispatch tasks to thread pool") 
{
    ThreadPool pool(4);
    std::atomic<int> counter = 0;

    pool.Dispatch([&] { ++counter; });
    pool.Dispatch([&] { ++counter; });
    pool.Wait();

    REQUIRE(counter == 2);
}

TEST_CASE("Stop prevents further task execution") 
{
    ThreadPool pool(4);
    std::atomic<int> counter = 0;

    pool.Dispatch([&] { ++counter; });
    pool.Stop();
    pool.Dispatch([&] { ++counter; });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    REQUIRE(counter == 1);
}

TEST_CASE("Task exceptions do not stop thread pool") 
{
    ThreadPool pool(4);
    std::atomic<int> counter = 0;

    pool.Dispatch([&] { throw std::runtime_error("Test exception"); });
    pool.Dispatch([&] { ++counter; });
    pool.Wait();

    REQUIRE(counter == 1);
}

TEST_CASE("Multiple threads execute tasks") 
{
    ThreadPool pool(4);
    std::atomic<int> counter = 0;

    for (int i = 0; i < 100; ++i) 
    {
        pool.Dispatch([&] { ++counter; });
    }

    pool.Wait();
    REQUIRE(counter == 100);
}

TEST_CASE("Stop gracefully stops the thread pool") 
{
    ThreadPool pool(4);
    pool.Stop();

    REQUIRE_NOTHROW(pool.Dispatch([] {  }));
    REQUIRE_NOTHROW(pool.Wait());
}
