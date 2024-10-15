#ifdef __linux__
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <system_error>
#include <cstring>
#include <numeric>
#include "Pipe.h"

std::string HandleAdd(const std::vector<int>& numbers)
{
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    return "sum is " + std::to_string(sum);
}

std::string HandleLongestWord(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return "Failed to open file";
    }

    std::string longest, word;
    while (file >> word)
    {
        if (word.size() > longest.size())
        {
            longest = word;
        }
    }

    return longest.empty() ? "" : longest;
}

int main()
{
    Pipe parentToChild;
    Pipe childToParent;

    pid_t pid = fork();
    if (pid == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Fork failed");
    }

    if (pid == 0)
    {
        close(parentToChild.WriteFd());
        close(childToParent.ReadFd());

        while (true)
        {
            std::string command = ReceiveMessage(parentToChild.ReadFd());

            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd;

            if (cmd == "add")
            {
                std::vector<int> numbers;
                int num;
                while (iss >> num)
                {
                    numbers.push_back(num);
                }
                std::string result = HandleAdd(numbers);
                SendMessage(childToParent.WriteFd(), result);
            }
            else if (cmd == "longest_word")
            {
                std::string filename;
                iss >> filename;
                std::string result = HandleLongestWord(filename);
                SendMessage(childToParent.WriteFd(), result);
            }
            else if (cmd == "exit")
            {
                break;
            }
        }

        close(parentToChild.ReadFd());
        close(childToParent.WriteFd());
        return 0;
    }
    else
    {
        close(parentToChild.ReadFd());
        close(childToParent.WriteFd());

        std::string line;
        while (true)
        {
            std::getline(std::cin, line);
            if (line == "exit")
            {
                SendMessage(parentToChild.WriteFd(), line);
                break;
            }

            SendMessage(parentToChild.WriteFd(), line);
            std::string result = ReceiveMessage(childToParent.ReadFd());
            std::cout << result << std::endl;
        }

        close(parentToChild.WriteFd());
        close(childToParent.ReadFd());
    }

    return 0;
}
#endif // __linux__