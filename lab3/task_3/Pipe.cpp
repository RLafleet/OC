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

#endif // __linux__
