#ifdef __linux__
#include "SignalHandlerLinux.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <csignal>
#include <filesystem>

namespace fs = std::filesystem;

void SignalHandlerLinux::SetupSignalHandlers() {
    struct sigaction action;
    action.sa_handler = SignalHandlerLinux::SignalHandlerFunc;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGTERM, &action, nullptr);
}

int SignalHandlerLinux::Run() {
    std::ofstream tempFile("temp.txt");
    if (!tempFile.is_open()) {
        std::cerr << "Failed to create temp.txt" << std::endl;
        return 1;
    }
    tempFile.close();

    for (int i = 0; i <= 100; ++i) {
        if (exitRequested) break;
        std::cout << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (fs::exists("temp.txt")) {
        fs::remove("temp.txt");
        std::cout << "File temp.txt deleted successfully." << std::endl;
    }

    std::cout << "Done" << std::endl;
    return 0;
}

void SignalHandlerLinux::SignalHandlerFunc(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        std::cout << "Received signal " << signum << ", exiting gracefully." << std::endl;
        exitRequested = true;
    }
}

#endif //__linux__