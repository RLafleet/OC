#include <iostream>
#ifdef __linux__
#include "SignalHandlerLinux.h"  // изменено на .h файл
#elif defined(_WIN32)
#include "SignalHandlerWindows.h"  // изменено на .h файл
#else
#error "Unsupported OS"
#endif

int main() {
    SignalHandler* handler;

#ifdef __linux__
    handler = new SignalHandlerLinux();
#elif defined(_WIN32)
    handler = new SignalHandlerWindows();
#endif

    handler->SetupSignalHandlers();
    int result = handler->Run();

    delete handler; // Освобождение памяти
    return result;
}
