#ifdef __linux__
#include "Pipe.h"
#include <iostream>
#include <string>

Pipe::Pipe()
{
    if (pipe(fds) == -1)
    {
        throw std::system_error(errno, std::generic_category(), "Pipe creation failed");
    }
}

Pipe::~Pipe()
{
    close(fds[0]);
    close(fds[1]);
}

int Pipe::ReadFd() const
{
    return fds[0];
}

int Pipe::WriteFd() const
{
    return fds[1];
}

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
    if (bytesRead == -1) {
        throw std::system_error(errno, std::generic_category(), "Read from pipe failed");
    }
    return std::string(buffer);
}
#endif // __linux__
