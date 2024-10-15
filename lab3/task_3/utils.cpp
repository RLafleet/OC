#ifdef __linux__

#include <system_error>
#include <unistd.h>
#include <cstring>

void SendMessage(int fd, const std::string& message)
{
    if (write(fd, message.c_str(), message.size() + 1) == -1) {
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
#endif //__linux__