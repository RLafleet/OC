#include <windows.h>
#include <cstdlib>
#include <ctime>

COLORREF g_ellipseColor = RGB(0, 0, 255);

void ChangeEllipseColor()
{
    g_ellipseColor = RGB(rand() % 256, rand() % 256, rand() % 256);
}

void DrawEllipse(HWND hwnd, HDC hdc)
{
    RECT rect;
    GetClientRect(hwnd, &rect);

    HBRUSH hBrush = CreateSolidBrush(g_ellipseColor);

    FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    SelectObject(hdc, hBrush);

    Ellipse(hdc, rect.left + 10, rect.top + 10,
        rect.right - 10, rect.bottom - 10);

    DeleteObject(hBrush);
}
