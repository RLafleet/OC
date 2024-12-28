#include <thread>
#include <vector>
#include <sstream>
#include "Socket.h"

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <arpa/inet.h> 
#endif

void HandleClient(int client_fd) 
{
    try 
    {
        SocketRAII client(client_fd);
        // почему 1024
        // что будет с программой если больше 1024. И исправить проблему, если будет
        char buffer[1024];
        while (true) 
        {
            int received = recv(client.get(), buffer, sizeof(buffer) - 1, 0);
            if (received <= 0) break;

            buffer[received] = '\0';
            std::istringstream stream(buffer);
            std::string command;
            stream >> command;

            std::string response;
            if (command == "+") 
            {
                int sum = 0, num;
                while (stream >> num)
                {
                    sum += num;
                }
                response = std::to_string(sum);
            }
            else if (command == "-") 
            {
                int difference, num;
                if (stream >> difference) 
                {
                    while (stream >> num)
                    {
                        difference -= num;
                    }
                    response = std::to_string(difference);
                }
                else 
                {
                    response = "Error: Invalid input";
                }
            }
            else 
            {
                response = "Error: Unknown command";
            }

            send(
                client.get(), 
                response.c_str(), 
                static_cast<int>(response.size()), 
                0
            );
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Client error: " << e.what() << std::endl;
    }
}

void StartServer(int port) 
{
    try 
    {
        SocketRAII server_socket(static_cast<int>(socket(AF_INET, SOCK_STREAM, 0)));
        sockaddr_in server_addr = {};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port); // выяснить для чего именно и какие напарники

        bind(server_socket.get(), (sockaddr*)&server_addr, sizeof(server_addr));
        // если завершится неудачей, то подумать над этим
        listen(server_socket.get(), 5); // почему именно 5

        while (true) 
        {
            // CC
            int client_fd = static_cast<int>(accept(server_socket.get(), nullptr, nullptr));
            std::thread(HandleClient, client_fd).detach(); // можно ли сделать лучше?
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}

void StartClient(const std::string& address, int port) 
{
    try {
        SocketRAII client_socket(static_cast<int>(socket(AF_INET, SOCK_STREAM, 0)));
        sockaddr_in server_addr = {};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr);

        connect(client_socket.get(), (sockaddr*)&server_addr, sizeof(server_addr));

        std::string command;
        while (true) 
        {
            std::cout << "> ";
            std::getline(std::cin, command);
            if (command.empty())
            {
                continue;
            }

            send(client_socket.get(), command.c_str(), static_cast<int>(command.size()), 0);

            char buffer[1024];
            int received = recv(client_socket.get(), buffer, sizeof(buffer) - 1, 0);
            if (received > 0) 
            {
                buffer[received] = '\0';
                std::cout << "Response: " << buffer << std::endl;
            }
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Client error: " << e.what() << std::endl;
    }
}

