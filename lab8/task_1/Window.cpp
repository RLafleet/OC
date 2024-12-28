#include <windows.h>
#include "window.h"
#include "graphics.h"
#include "HandleWrapper.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HMENU hMenu;

    switch (uMsg)
    {
    case WM_CREATE:
        CreateMenus(hwnd);
        break;

    case WM_COMMAND:
        HandleCommand(hwnd, LOWORD(wParam));
        break;

    case WM_LBUTTONDOWN:
        ChangeEllipseColor();
        InvalidateRect(hwnd, nullptr, TRUE);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        // нельзя вызывать delete object, для удаления контекста устройства используется deleteDC
        // тут нельзя raii обертку
        HandleWrapper<HDC> hdc(BeginPaint(hwnd, &ps));
        DrawEllipse(hwnd, hdc.get());
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_SIZE:
        InvalidateRect(hwnd, nullptr, TRUE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void RegisterWindowClass(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] = L"Window Class";

    // что такое оконная процедура
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
}

HWND CreateMainWindow(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] = L"Window Class";

    return CreateWindowEx(
        0, CLASS_NAME, L"Windows Graphics Application", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // style
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // pos
        nullptr, nullptr, hInstance, nullptr);
}

void CreateMenus(HWND hwnd)
{
    HandleWrapper<HMENU> hFileMenu(CreateMenu());
    HandleWrapper<HMENU> hHelpMenu(CreateMenu());

    // константы
    AppendMenu(hFileMenu.get(), MF_STRING, 1, L"Exit");
    AppendMenu(hHelpMenu.get(), MF_STRING, 2, L"About");

    HandleWrapper<HMENU> hMenu(CreateMenu());
    AppendMenu(hMenu.get(), MF_POPUP, (UINT_PTR)hFileMenu.release(), L"File");
    AppendMenu(hMenu.get(), MF_POPUP, (UINT_PTR)hHelpMenu.release(), L"Help");

    SetMenu(hwnd, hMenu.release());
}

void HandleCommand(HWND hwnd, int commandId)
{
    switch (commandId)
    {
    case 1:
        if (MessageBox(hwnd, L"Are you sure you want to exit?", L"Exit Confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES)
        {
            //так нельзя, сначало окно
            PostQuitMessage(0);
        }
        break;

    case 2: 
        MessageBox(hwnd, L"Windows Graphics Application\nAuthor: Your Name", L"About", MB_OK | MB_ICONINFORMATION);
        break;

    default:
        break;
    }
}
