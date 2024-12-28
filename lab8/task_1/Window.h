#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void RegisterWindowClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);
void CreateMenus(HWND hwnd);
void HandleCommand(HWND hwnd, int commandId);

#endif // WINDOW_H
