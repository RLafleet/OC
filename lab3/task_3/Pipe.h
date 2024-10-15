#ifndef PIPE_H
#define PIPE_H
#ifdef __linux__

#include <system_error>
#include <unistd.h>
#include <cstring>

class Pipe
{
public:
    Pipe();
    ~Pipe();

    int ReadFd() const;
    int WriteFd() const;

private:
    int fds[2];
};

void SendMessage(int fd, const std::string& message);
std::string ReceiveMessage(int fd);

#endif // PIPE_H
#endif // __linux__
