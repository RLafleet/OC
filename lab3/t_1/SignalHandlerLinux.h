#ifndef SIGNAL_HANDLER_LINUX_H
#define SIGNAL_HANDLER_LINUX_H

#include "SignalHandler.h"

class SignalHandlerLinux : public SignalHandler 
{
public:
    void SetupSignalHandlers() override;
    int Run() override;

private:
    static void SignalHandlerFunc(int signum);
};

#endif // SIGNAL_HANDLER_LINUX_H
