#include "Server.h"
#include <string>

int main(int argc, char* argv[]) 
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    try 
    {
        if (argc == 2) 
        {
            // выяснить что такое порт и зачем он нужен и какие они бывают
            // как сервер 
            // как понимать какой клиент должен получить responce
            int port = std::stoi(argv[1]);
            StartServer(port);
        }
        else if (argc == 3) 
        {
            std::string address = argv[1];
            int port = std::stoi(argv[2]);
            StartClient(address, port);
        }
        else 
        {
            std::cerr << "Usage:\n"
                << "  Server: calc PORT\n"
                << "  Client: calc ADDRESS PORT\n";
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
