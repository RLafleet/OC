#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <atomic>

class SignalHandler {
public:
    static std::atomic_bool exitRequested;  

    virtual void SetupSignalHandlers() = 0;
    virtual int Run() = 0;
    virtual ~SignalHandler() = default;
};

#endif // SIGNAL_HANDLER_H