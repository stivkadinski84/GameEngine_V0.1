#define UNICODE
#define _UNICODE
#include "platform/windows/window.h"

static bool isWindowRunning = false;
static HWND globalWindowHandle = NULL;
static HDC globalWindowDeviceContext = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        isWindowRunning = false;
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        isWindowRunning = false;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    extern int main(int argc, char **argv);
    return main(__argc, __argv);
}

bool BootWindow(const char *title, int width, int height, HWND *outHwnd, HDC *outHdc)
{

    HINSTANCE hInstance = GetModuleHandle(NULL);

    wchar_t wTitle[256];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, wTitle, 256);

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GameWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    globalWindowHandle = CreateWindowEx(
        0,
        L"GameWindow", wTitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInstance, NULL);

    if (!globalWindowHandle)
        return false;

    globalWindowDeviceContext = GetDC(globalWindowHandle);

    if (!globalWindowDeviceContext)
    {
        DestroyWindow(globalWindowHandle);
        globalWindowHandle = NULL;
        return false;
    }

    isWindowRunning = true;

    if (outHwnd)
        *outHwnd = globalWindowHandle;
    if (outHdc)
        *outHdc = globalWindowDeviceContext;

    return true;
}

bool WindowIsRunning(void)
{
    return isWindowRunning;
}

void WindowUpdate(void)
{
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Drawing functions on window here...
}

void WindowClose(void)
{
    if (globalWindowHandle && globalWindowDeviceContext)
        ReleaseDC(globalWindowHandle, globalWindowDeviceContext);
    if (globalWindowHandle)
        DestroyWindow(globalWindowHandle);
}