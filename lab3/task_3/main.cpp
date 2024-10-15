#ifdef __linux__
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <unistd.h>
#include <system_error>
#include "Pipe.h"
#include "utils.cpp"
#include <span>

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
            std::string command = ReceiveString(parentToChild.ReadFd()); 

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

                int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
                SendSpan(childToParent.WriteFd(), std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(&sum), sizeof(sum))); 
            }
            else if (cmd == "longestWord") 
            {
                std::string filename;
                iss >> filename;
                std::string result = HandleLongestWord(filename);
                SendString(childToParent.WriteFd(), result);  
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
                SendString(parentToChild.WriteFd(), line);  
                break;
            }

            SendString(parentToChild.WriteFd(), line);  

            if (line.find("add") == 0) 
            {
                int result;
                std::vector<uint8_t> resultBuffer = ReceiveSpan(childToParent.ReadFd(), sizeof(result));  
                std::memcpy(&result, resultBuffer.data(), sizeof(result));
                std::cout << "sum is " << result << std::endl;
            }
            else if (line.find("longestWord") == 0) 
            {
                std::string result = ReceiveString(childToParent.ReadFd());
                std::cout << result << std::endl;
            }
        }

        close(parentToChild.WriteFd());
        close(childToParent.ReadFd());
    }

    return 0;
}

#endif // __linux__