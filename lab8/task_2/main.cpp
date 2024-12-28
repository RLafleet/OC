#include "Display.h"
#include "Snowflake.h"
#include "EventHandler.h"
#include <unistd.h>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;        
const int WINDOW_HEIGHT = 600;       
const int SNOWFLAKE_COUNT = 70;      
const int FRAME_DELAY_MICROSECONDS = 30000; 

void RunSnowfallSimulation() 
{
    DisplayRAII displayRAII;
    Display* display = displayRAII.get();

    srand(static_cast<unsigned>(time(nullptr))); 

    Window window = CreateWindow(display, WINDOW_WIDTH, WINDOW_HEIGHT);
    GC gc = XCreateGC(display, window, 0, nullptr);
    // raii обертку для графического контекста
    // добавить псевдослучайное движение в сторону или гармоническое колебание 

    auto snowflakes = InitializeSnowflakes(SNOWFLAKE_COUNT, WINDOW_WIDTH, WINDOW_HEIGHT);

    bool running = true;
    while (running) 
    {
        running = ProcessEvents(display);
        DrawSnowflakes(display, window, gc, snowflakes, WINDOW_WIDTH, WINDOW_HEIGHT);
        XFlush(display);
        usleep(FRAME_DELAY_MICROSECONDS);
    }

    XFreeGC(display, gc);
}

int main() 
{
    try 
    {
        RunSnowfallSimulation();
    }
    catch (const std::exception& e) 
    {
        fprintf(stderr, "Error: %s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
