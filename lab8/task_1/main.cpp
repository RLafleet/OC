#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Window.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    RegisterWindowClass(hInstance);
    HWND hwnd = CreateMainWindow(hInstance);

    if (!hwnd)
    {
        return 0;
    }
    
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

//клики только по эллипсу
//курсор

