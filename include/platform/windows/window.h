#ifndef WINDOW_H
#define WINDOW_H
#include <Windows.h>
#include <stdint.h>
#include <stdbool.h>

bool BootWindow(const char *title, int width, int height, HWND *outHwnd, HDC *outHdc);

bool WindowIsRunning(void);

void WindowUpdate(void);

void WindowClose(void);

#endif