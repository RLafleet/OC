#include "Snowflake.h"
#include <cstdlib>
#include <ctime>
#include <X11/Xlib.h>

int GetRandomNumber(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

std::vector<Snowflake> InitializeSnowflakes(int count, int width, int height) 
{
    std::vector<Snowflake> snowflakes(count);

    for (auto& flake : snowflakes) 
    {
        flake.x = GetRandomNumber(0, width);
        flake.y = GetRandomNumber(0, height);
        flake.radius = GetRandomNumber(2, 5);
        flake.speed = GetRandomNumber(1, 3);
    }

    return snowflakes;
}

// выделить может какой-нибудь класс? подумать и выделить
void DrawSnowflakes(Display* display, Window window, GC gc,
    const std::vector<Snowflake>& snowflakes,
    int width, int height)
{
    XSetForeground(display, gc, 0x404040); 
    XFillRectangle(display, window, gc, 0, 0, width, height);

    XSetForeground(display, gc, WhitePixel(display, DefaultScreen(display)));

    // избавиться от const_cast
    for (auto& flake : const_cast<std::vector<Snowflake>&>(snowflakes))
    {
        XFillArc(display, window, gc,
            flake.x - flake.radius,
            flake.y - flake.radius,
            flake.radius * 2,
            flake.radius * 2,
            0, 360 * 64);
        // посмотреть, есть ли эллипс

        flake.y += flake.speed;

        if (flake.y > height)
        {
            flake.y = 0;
            flake.x = GetRandomNumber(0, width);
        }
    }
}

