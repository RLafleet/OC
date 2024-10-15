#ifndef PIPE_H
#define PIPE_H
#ifdef __linux__

#include <system_error>
#include <unistd.h>
#include <cstring>

// обертка с одним файловым дескриптором
class Pipe
{
public:
    Pipe();
    ~Pipe();

    int ReadFd() const;
    int WriteFd() const;

    Pipe(const Pipe&) = delete;
    Pipe& operator=(const Pipe&) = delete;
    Pipe(Pipe&&) = delete;
    Pipe& operator=(Pipe&&) = delete;
private:
    int fds[2];
};

#endif // PIPE_H
#endif // __linux__
