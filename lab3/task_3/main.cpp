#ifdef __linux__
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include "Pipe.h"
#include "utils.cpp"

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

                int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
                SendData(childToParent.WriteFd(), sum);
            }
            else if (cmd == "longestWord") 
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

            if (line.find("add") == 0) 
            {
                int result = ReceiveData<int>(childToParent.ReadFd());  
                std::cout << "sum is " << result << std::endl;
            }
            else if (line.find("longestWord") == 0) 
            {
                std::string result = ReceiveMessage(childToParent.ReadFd());
                std::cout << result << std::endl;
            }
        }

        close(parentToChild.WriteFd());
        close(childToParent.ReadFd());
    }

    return 0;
}

#endif // __linux__