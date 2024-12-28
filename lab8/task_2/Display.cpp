#include "Display.h"

DisplayRAII::DisplayRAII(const char* displayName) 
{
    display = XOpenDisplay(displayName);
    if (!display) 
    {
        throw std::runtime_error("Failed to open X display");
    }
}

DisplayRAII::~DisplayRAII() 
{
    if (display) 
    {
        XCloseDisplay(display);
    }
}

Display* DisplayRAII::get() const   
{
    return display;
}

Window CreateWindow(Display* display, int width, int height) 
{
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen),
        0, 0, width, height, 1,
        BlackPixel(display, screen),
        WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    return window;
}
