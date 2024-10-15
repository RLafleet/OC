#ifdef __linux__
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <thread>
#include <chrono>

std::vector<pid_t> children;

void SignalHandler(int signum) 
{
    std::cout << "Child process " << getpid() << " received signal " << signum << "\n";
}

void ChildProcess()
{
    signal(SIGTERM, SignalHandler);
    signal(SIGINT, SignalHandler);

    pid_t pid = getpid();
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Child process " << pid << " is still alive\n";
    }
}

int main()
{
    std::string command;

    while (true)
    {
        std::cin >> command;

        if (command == "exit")
        {
            std::cout << "Exiting...\n";
            break;
        }
        else if (command == "spawn")
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                ChildProcess();
                return 0;
            }
            else if (pid > 0)
            {
                children.push_back(pid);
                std::cout << "Spawned child process with PID: " << pid << "\n";
            }
            else
            {
                std::cerr << "Fork failed!\n";
            }
        }
        else if (command == "kill")
        {
            if (!children.empty())
            {
                pid_t pid = children.front();
                children.erase(children.begin());
                std::cout << "Kill child process with PID: " << pid << "\n";
                kill(pid, SIGTERM);
                waitpid(pid, nullptr, 0);
            }
            else
            {
                std::cout << "No children to kill\n";
            }
        }
        else
        {
            std::cout << "Unknown command: " << command << "\n";
        }
    }

    for (pid_t pid : children)
    {
        std::cout << "Terminating remaining after exit child process with PID: " << pid << "\n";
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0);
    }

    return 0;
}
#endif // __linux__