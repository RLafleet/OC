#ifdef __linux__
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <system_error>
#include <cstring>
#include <numeric>

// RAII-обёртка для файловых дескрипторов
class Pipe {
public:
    Pipe() {
        if (pipe(fds) == -1) {
            throw std::system_error(errno, std::generic_category(), "Pipe creation failed");
        }
    }

    ~Pipe() {
        close(fds[0]);
        close(fds[1]);
    }

    int read_fd() const { return fds[0]; }
    int write_fd() const { return fds[1]; }

private:
    int fds[2];
};

// Функция для отправки данных через pipe
void send_message(int fd, const std::string& message) {
    if (write(fd, message.c_str(), message.size() + 1) == -1) {
        throw std::system_error(errno, std::generic_category(), "Write to pipe failed");
    }
}

// Функция для чтения данных из pipe
std::string receive_message(int fd) {
    char buffer[256];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        throw std::system_error(errno, std::generic_category(), "Read from pipe failed");
    }
    return std::string(buffer);
}

// Функция для выполнения команды add
std::string handle_add(const std::vector<int>& numbers) {
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    return "sum is " + std::to_string(sum);
}

// Функция для выполнения команды longest_word
std::string handle_longest_word(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "Failed to open file";
    }

    std::string longest, word;
    while (file >> word) {
        if (word.size() > longest.size()) {
            longest = word;
        }
    }

    return longest.empty() ? "" : longest;
}

int main() {
    Pipe parent_to_child;
    Pipe child_to_parent;

    pid_t pid = fork();
    if (pid == -1) {
        throw std::system_error(errno, std::generic_category(), "Fork failed");
    }

    if (pid == 0) {
        // Дочерний процесс
        close(parent_to_child.write_fd());
        close(child_to_parent.read_fd());

        while (true) {
            std::string command = receive_message(parent_to_child.read_fd());

            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd;

            if (cmd == "add") {
                std::vector<int> numbers;
                int num;
                while (iss >> num) {
                    numbers.push_back(num);
                }
                std::string result = handle_add(numbers);
                send_message(child_to_parent.write_fd(), result);
            }
            else if (cmd == "longest_word") {
                std::string filename;
                iss >> filename;
                std::string result = handle_longest_word(filename);
                send_message(child_to_parent.write_fd(), result);
            }
            else if (cmd == "exit") {
                break;
            }
        }

        close(parent_to_child.read_fd());
        close(child_to_parent.write_fd());
        _exit(0);
    }
    else {
        // Родительский процесс
        close(parent_to_child.read_fd());
        close(child_to_parent.write_fd());

        std::string line;
        while (true) {
            std::getline(std::cin, line);
            if (line == "exit") {
                send_message(parent_to_child.write_fd(), line);
                break;
            }

            send_message(parent_to_child.write_fd(), line);
            std::string result = receive_message(child_to_parent.read_fd());
            std::cout << result << std::endl;
        }

        close(parent_to_child.write_fd());
        close(child_to_parent.read_fd());
    }

    return 0;
}
#endif // __linux__