#include <iostream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

// Перемещаемой
// в handle client, сразу же инициализаировать SockerRAII
class SocketRAII 
{
    int socket_fd;
public:
    explicit SocketRAII(int fd) : socket_fd(fd) 
    {
        if (socket_fd < 0) 
        {
            throw std::runtime_error("Socket creation failed");
        }
    }
    ~SocketRAII() 
    {
#ifdef _WIN32
        closesocket(socket_fd);
#else
        close(socket_fd);
#endif
    }

    SocketRAII(const SocketRAII&) = delete;             
    SocketRAII& operator=(const SocketRAII&) = delete;  

    SocketRAII(SocketRAII&&) = delete;                  
    SocketRAII& operator=(SocketRAII&&) = delete;       

    int get() const { return socket_fd; }
};
