#define CATCH_CONFIG_MAIN
#include "../../../ood/Catch2/catch.hpp"

#include "../task_2/BgThreadDispatcher.h"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <sstream>

TEST_CASE("Dispatch tasks and wait for completion")
{
    BgThreadDispatcher dispatcher;
    std::atomic<int> counter = 0;

    dispatcher.Dispatch([&] { ++counter; });
    dispatcher.Dispatch([&] { ++counter; });
    dispatcher.Wait();

    REQUIRE(counter == 2);
}

TEST_CASE("Stop prevents further task execution")
{
    BgThreadDispatcher dispatcher;
    std::atomic<int> counter = 0;

    dispatcher.Dispatch([&] { ++counter; });
    dispatcher.Wait();
    dispatcher.Stop();
    dispatcher.Dispatch([&] { ++counter; });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    REQUIRE(counter == 1);
}

TEST_CASE("Task exceptions do not stop dispatcher")
{
    BgThreadDispatcher dispatcher;
    std::atomic<int> counter = 0;

    dispatcher.Dispatch([&] { throw std::runtime_error("Test exception"); });
    dispatcher.Dispatch([&] { ++counter; });
    dispatcher.Wait();

    REQUIRE(counter == 1);
}

TEST_CASE("No tasks to wait for")
{
    BgThreadDispatcher dispatcher;
    dispatcher.Wait();
    REQUIRE(true);
}

TEST_CASE("Dispatch after Wait works")
{
    BgThreadDispatcher dispatcher;
    std::atomic<int> counter = 0;

    dispatcher.Wait();
    dispatcher.Dispatch([&] { ++counter; });
    dispatcher.Wait();

    REQUIRE(counter == 1);
}

TEST_CASE("Stop without dispatching tasks")
{
    BgThreadDispatcher dispatcher;
    dispatcher.Stop();
    REQUIRE(true);
}

TEST_CASE("Multiple tasks in sequence")
{
    BgThreadDispatcher dispatcher;
    std::atomic<int> counter = 0;

    for (int i = 0; i < 100; ++i)
    {
        dispatcher.Dispatch([&] { ++counter; });
    }
    dispatcher.Wait();
    REQUIRE(counter == 100);
}

TEST_CASE("Output 1 and ! in correct order") {
    BgThreadDispatcher dispatcher;
    std::ostringstream oss;
    {
        std::ostringstream& sync_out = oss;  
        dispatcher.Dispatch([&] { sync_out << "1"; });
        dispatcher.Wait();
        sync_out << "!"; 
    }
    REQUIRE(oss.str() == "1!");  
}

TEST_CASE("Output combinations of !123, 1!23, 12!3") {
    BgThreadDispatcher dispatcher;
    std::ostringstream oss;
    {
        std::ostringstream& sync_out = oss;  
        dispatcher.Dispatch([&] { sync_out << "1"; });
        dispatcher.Dispatch([&] { sync_out << "2"; });
        sync_out << "!"; 
        dispatcher.Wait();
        dispatcher.Dispatch([&] { sync_out << "3"; });
        dispatcher.Wait();
    }

    std::string output = oss.str();
    REQUIRE((output == "!123" || output == "1!23" || output == "12!3"));
}

TEST_CASE("Output ! when no tasks") {
    BgThreadDispatcher dispatcher;
    std::ostringstream oss;
    dispatcher.Wait(); 
    oss << "!"; 
    REQUIRE((oss.str() == "!"));
}

TEST_CASE("Output 1! or ! after Stop") {
    BgThreadDispatcher dispatcher;
    std::ostringstream oss;
    {
        std::ostringstream& sync_out = oss;  
        dispatcher.Dispatch([&] { sync_out << "1"; });
        std::this_thread::sleep_for(std::chrono::seconds(1));
        dispatcher.Stop();
        dispatcher.Dispatch([&] { sync_out << "2"; });
        dispatcher.Wait();
        sync_out << "!";
    }

    std::string output = oss.str();
    REQUIRE((output == "1!" || output == "!"));
}
