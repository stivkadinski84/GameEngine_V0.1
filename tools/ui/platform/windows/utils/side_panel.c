#include "side_panel.h"
#include "buttons.h"

HWND Create_SidePanel(HWND parentWindow)
{
    HWND side_panel = CreateWindowExW(
        0,
        L"STATIC",
        L"",
        WS_CHILD | WS_VISIBLE | SS_WHITERECT,
        800, 0, 200, 600,
        parentWindow,
        NULL,
        GetModuleHandleW(NULL),
        NULL);

    if (side_panel == NULL)
    {
        return 0;
    }

    // Create the button inside the side panel.
    if (CreateButton(side_panel) == NULL)
    {
        DestroyWindow(side_panel);
        return NULL;
    }
}