#include "EventHandler.h"
#include <X11/Xlib.h>

bool ProcessEvents(Display* display)
{
    while (XPending(display)) 
    {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == KeyPress)
        {
            return false;
        }
    }

    return true;
}
