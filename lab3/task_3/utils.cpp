#ifdef __linux__

#include <system_error>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <span>
#include <bit>
#include <cstring>
#include <unistd.h>
#include <system_error>
#include <numeric>
#include <fstream>

template <typename T>
void SendData(int fd, const T& data) 
{
    auto buffer = std::bit_cast<const char*>(&data);
    if (write(fd, buffer, sizeof(T)) == -1) 
    {
        throw std::system_error(errno, std::generic_category(), "Write to pipe failed");
    }
}

template <typename T>
T ReceiveData(int fd) 
{
    T data;
    auto buffer = std::bit_cast<char*>(&data);
    if (read(fd, buffer, sizeof(T)) == -1) 
    {
        throw std::system_error(errno, std::generic_category(), "Read from pipe failed");
    }
    return data;
}

void SendMessage(int fd, const std::string& message) 
{
    if (write(fd, message.c_str(), message.size() + 1) == -1) 
    {
        throw std::system_error(errno, std::generic_category(), "Write to pipe failed");
    }
}

std::string ReceiveMessage(int fd) 
{
    char buffer[256];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) 
    {
        throw std::system_error(errno, std::generic_category(), "Read from pipe failed");
    }
    return std::string(buffer);
}

std::string HandleAdd(const std::vector<int>& numbers) 
{
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    return "sum is " + std::to_string(sum);
}

std::string HandleLongestWord(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file) 
    {
        return "Error: unable to open file";
    }

    std::string longestWord, word;
    while (file >> word) 
    {
        if (word.length() > longestWord.length()) 
        {
            longestWord = word;
        }
    }

    return longestWord.empty() ? "" : longestWord;
}

#endif //__linux__