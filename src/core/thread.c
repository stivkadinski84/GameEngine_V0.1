#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "platform/windows/window.h"
#include "core/engine.h"
#include "graphics/bitmap.h"

DWORD WINAPI newThread(LPVOID lpParam)
{
    EngineClass *engine = (EngineClass *)lpParam;

    // Setting windows timer resolution to 1ms
    timeBeginPeriod(1);

    LARGE_INTEGER frequency;
    LARGE_INTEGER timePoint1, timePoint2;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&timePoint1);

    // Setting time counting frames to 60 FPS
    const float targetFrameTime = 1.0f / 60.0f;

    while (engine->isRunning)
    {
        QueryPerformanceCounter(&timePoint2);
        float fElapsedTime = (float)(timePoint2.QuadPart - timePoint1.QuadPart) / (float)frequency.QuadPart;
        timePoint1 = timePoint2;

        // Game Update
        if (engine->OnUserUpdateGame != NULL)
        {
            if (!engine->OnUserUpdateGame(engine, fElapsedTime))
            {
                engine->isRunning = 0;
            }
        }

        BitMap_Update(
            engine->hdc,            // HDC from GetDC(engine->hwnd)
            engine->hwnd,           // Window handle
            engine->bitmap_pixel_buffer,   // Your raw uint32_t pixel buffer
            engine->renderer.width, // Buffer width
            engine->renderer.height // Buffer height
        );

        LARGE_INTEGER frameEnd;
        QueryPerformanceCounter(&frameEnd);
        float frameWorkTime = (float)(frameEnd.QuadPart - timePoint2.QuadPart) / (float)frequency.QuadPart;

        if (frameWorkTime < targetFrameTime)
        {
            DWORD sleepMs = (DWORD)((targetFrameTime - frameWorkTime) * 1000.0f);
            if (sleepMs > 0)
            {
                Sleep(sleepMs); // Yield CPU for remaining frame duration
            }
        }
    }

    timeEndPeriod(1);
    return 0;
}

// The name must remain createThread because CreateThread can cause error due to being same name as the Windows API function CreateThread();
HANDLE createThread(EngineClass *engine)
{
    HANDLE hThread = CreateThread(
        NULL,
        0,
        newThread,
        engine,
        0,
        NULL);
    return hThread;
}