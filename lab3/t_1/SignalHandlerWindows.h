#ifndef SIGNAL_HANDLER_WINDOWS_H
#define SIGNAL_HANDLER_WINDOWS_H

#include "SignalHandler.h"

class SignalHandlerWindows : public SignalHandler 
{
public:
    void SetupSignalHandlers() override;
    int Run() override;

private:
    static void SignalHandlerFunc(int signum);
};

#endif // SIGNAL_HANDLER_WINDOWS_H
