#include <iostream>
#ifdef __linux__
#include "SignalHandlerLinux.h"  
#elif defined(_WIN32)
#include "SignalHandlerWindows.h"  
#else
#error "Unsupported OS"
#endif

// фабрику
// бизнес логика одна
int main() 
{
    SignalHandler* handler;

#ifdef __linux__
    handler = new SignalHandlerLinux();
#elif defined(_WIN32)
    handler = new SignalHandlerWindows();
#endif

    handler->SetupSignalHandlers();
    int result = handler->Run();

    delete handler; 
    return result;
}
