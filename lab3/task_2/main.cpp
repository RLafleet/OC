#ifdef __linux__
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <thread>
#include <chrono>

std::vector<pid_t> children;

void signalHandler(int signum) {
    std::cout << "Child process " << getpid() << " received signal " << signum << "\n";
    exit(0); // Завершение процесса при получении сигнала
}

void childProcess() {
    signal(SIGTERM, signalHandler); // Обработка сигнала завершения
    signal(SIGINT, signalHandler);  // Обработка сигнала прерывания

    pid_t pid = getpid();
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Пауза на 5 секунд
        std::cout << "Child process " << pid << " is still alive\n";
    }
}

int main() {
    std::string command;

    while (true) {
        std::cin >> command;

        if (command == "exit") {
            // Завершение работы программы
            std::cout << "Exiting...\n";
            break;
        }
        else if (command == "spawn") {
            // Создание дочернего процесса
            pid_t pid = fork();
            if (pid == 0) {
                // Код дочернего процесса
                childProcess();
                return 0;
            }
            else if (pid > 0) {
                // Родительский процесс добавляет pid в список
                children.push_back(pid);
                std::cout << "Spawned child process with PID: " << pid << "\n";
            }
            else {
                std::cerr << "Fork failed!\n";
            }
        }
        else if (command == "kill") {
            // Убийство первого дочернего процесса в списке
            if (!children.empty()) {
                pid_t pid = children.front();
                children.erase(children.begin());
                std::cout << "Killing child process with PID: " << pid << "\n";
                kill(pid, SIGTERM);
                waitpid(pid, nullptr, 0); // Ожидание завершения процесса
            }
            else {
                std::cout << "No children to kill\n";
            }
        }
        else {
            std::cout << "Unknown command: " << command << "\n";
        }
    }

    // Завершение всех оставшихся дочерних процессов при завершении программы
    for (pid_t pid : children) {
        std::cout << "Terminating remaining child process with PID: " << pid << "\n";
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0); // Ожидание завершения
    }

    return 0;
}
#endif // __linux__