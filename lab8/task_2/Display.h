#ifndef DISPLAY_H
#define DISPLAY_H

#include <X11/Xlib.h>
#include <stdexcept>

class DisplayRAII
{
public:
    explicit DisplayRAII(const char* displayName = nullptr);
    ~DisplayRAII();

    Display* get() const;

    DisplayRAII(const DisplayRAII&) = delete; 
    DisplayRAII& operator=(const DisplayRAII&) = delete; 

    DisplayRAII(DisplayRAII&&) = delete; 
    DisplayRAII& operator=(DisplayRAII&&) = delete; 

private:
    // не соглашение в стиле кодирование, убрать лишнюю проверку на null
    Display* display;
};

Window CreateWindow(Display* display, int width, int height);

#endif // DISPLAY_H
