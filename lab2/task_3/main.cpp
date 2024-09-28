#ifdef __linux__
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdexcept>

bool WaitForChild()
{
    int status;
    pid_t waited_pid;
    bool success = false;

    std::cout << "Enter the PID of the child process (or -1 to exit): ";
    while (!success)
    {
        std::cin >> waited_pid;

        if (waited_pid == -1)
        {
            std::cout << "Exiting..." << std::endl;
            return false;
        }

        if (waitpid(waited_pid, &status, 0) == -1)
        {
            std::cerr << "Error: Please enter a valid PID." << std::endl;
        }
        else
        {
            std::cout << "Child process " << waited_pid << " has terminated." << std::endl;
            success = true;
        }
    }

    return success;
}

int main()
{
    pid_t child_pid = fork();

    if (child_pid < 0)
    {
        std::cerr << "Error: Fork failed!" << std::endl;
        return 1;
    }
    else if (child_pid == 0)
    {
        return 0;
    }
    else
    {
        try
        {
            bool waitedSuccessfully = WaitForChild();

            if (!waitedSuccessfully)
            {
                throw std::runtime_error("Failed to wait for the child process.");
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    return 0;
}
#endif //__linux__