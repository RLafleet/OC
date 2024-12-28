#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

#include <vector>
#include <X11/Xlib.h> 

struct Snowflake 
{
    int x, y;
    int radius;
    int speed;
};

void DrawSnowflakes(Display* display, Window window, GC gc,
    const std::vector<Snowflake>& snowflakes,
    int width, int height);

std::vector<Snowflake> InitializeSnowflakes(int count, int width, int height);

#endif // SNOWFLAKE_H
