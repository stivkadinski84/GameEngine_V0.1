#include "buttons.h"

HWND CreateButton(HWND parentPanel)
{
    HWND open_button = CreateWindowExW(
        0,
        L"BUTTON",
        L"Open Sprite",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 40, 180, 35,
        parentPanel,
        (HMENU)(INT_PTR)1001,
        GetModuleHandleW(NULL),
        NULL);

    if (open_button == NULL)
    {
        return 0;
    }
}