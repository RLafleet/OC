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

#include <span>

void SendSpan(int fd, std::span<const uint8_t> data)
{
    if (write(fd, data.data(), data.size_bytes()) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to write to pipe");
    }
}

std::vector<uint8_t> ReceiveSpan(int fd, size_t size)
{
    std::vector<uint8_t> buffer(size);
    if (read(fd, buffer.data(), size) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to read from pipe");
    }
    return buffer;
}

void SendString(int fd, const std::string& str)
{
    size_t len = str.size();
    if (write(fd, &len, sizeof(len)) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to write string length to pipe");
    }

    if (write(fd, str.data(), len) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to write string to pipe");
    }
}

std::string ReceiveString(int fd)
{
    size_t len;
    std::vector<uint8_t> lenBuffer = ReceiveSpan(fd, sizeof(len));  
    std::memcpy(&len, lenBuffer.data(), sizeof(len));

    std::vector<uint8_t> strBuffer = ReceiveSpan(fd, len);          
    return std::string(reinterpret_cast<char*>(strBuffer.data()), len);
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