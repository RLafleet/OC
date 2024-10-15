#ifdef _WIN32
#include "SignalHandlerWindows.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <csignal>
#include <filesystem>
#include <windows.h>

void SignalHandlerWindows::SetupSignalHandlers() {
    signal(SIGINT, SignalHandlerWindows::SignalHandlerFunc);
    signal(SIGTERM, SignalHandlerWindows::SignalHandlerFunc);
}

int SignalHandlerWindows::Run() {
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

    if (std::filesystem::exists("temp.txt")) {
        std::filesystem::remove("temp.txt");
        std::cout << "File temp.txt deleted successfully." << std::endl;
    }

    std::cout << "Done" << std::endl;
    return 0;
}

void SignalHandlerWindows::SignalHandlerFunc(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        std::cout << "Received signal " << signum << ", exiting gracefully." << std::endl;
        exitRequested = true;
    }
}

#endif //WIN32  
