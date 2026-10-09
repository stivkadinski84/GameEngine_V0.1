#include <stdio.h>
#include "core/engine.h"
#include "platform/windows/window.h"
#include "graphics/renderer.h"
#include "graphics/bitmap.h"
#include "utils/constants.h"

EngineClass CreateEngine(const char *title, int width, int height)
{
    EngineClass engine = {0};

    // Creating the game window
    // Error Checking for game window creation
    if (!BootWindow(title, width, height, &engine.hwnd, &engine.hdc))
    {
        printf("Engine Window creation failed check window.h and window.c\n");
        engine.isRunning = 0;
        return engine;
    }

    // CREATING PIXEL_BUFFER HERE BEFORE CREATING THE RENDERER

    engine.bitmap_pixel_buffer = CreatePixelBuffer(width, height);

    if (engine.bitmap_pixel_buffer == NULL)
    {
        printf("Pixel buffer creation failed! Check bitmap.h and bitmap.c file\n");
        engine.isRunning = 0;
        return engine;
    }

    Create_BitMap(engine.hwnd, width, height);

    // Creating the renderer and passing the pixel_buffer to send drawing to window
    engine.renderer = CreateRenderer(engine.bitmap_pixel_buffer, width, height);

    engine.isRunning = 1;
    engine.OnUserCreateGame = NULL;
    engine.OnUserUpdateGame = NULL;
    engine.OnUserDestroyGame = NULL;

    return engine;
}

EngineClass CreateEngineForSpriteEditor(const char *title, int windowWidth, int windowHeight, int rendererWidth, int rendererHeight)
{
    EngineClass engine = {0};

    // Creating the game window
    // Error Checking for game window creation
    if (!BootWindow(title, windowWidth, windowHeight, &engine.hwnd, &engine.hdc))
    {
        printf("Engine for Sprite Editor Window creation failed check window.h and window.c\n");
        engine.isRunning = 0;
        return engine;
    }

    // CREATING PIXEL_BUFFER HERE BEFORE CREATING THE RENDERER

    engine.bitmap_pixel_buffer = CreatePixelBuffer(rendererWidth, rendererHeight);

    if (engine.bitmap_pixel_buffer == NULL)
    {
        printf("Pixel buffer creation failed! Check bitmap.h and bitmap.c file\n");
        engine.isRunning = 0;
        return engine;
    }

    Create_BitMap(engine.hwnd, rendererWidth, rendererHeight);

    // Creating the renderer and passing the pixel_buffer to send drawing to window
    engine.renderer = CreateRenderer(engine.bitmap_pixel_buffer, rendererWidth, rendererHeight);

    engine.isRunning = 1;
    engine.OnUserCreateGame = NULL;
    engine.OnUserUpdateGame = NULL;
    engine.OnUserDestroyGame = NULL;

    return engine;
}

void StartEngine(EngineClass *engine)
{
    if (engine == NULL)
        return;

    // Initialize the game engine before starting the thread
    if (engine->OnUserCreateGame != NULL)
    {
        if (!engine->OnUserCreateGame(engine))
        {
            engine->isRunning = 0;
            return;
        }
    }

    engine->hThread = createThread(engine);

    if (engine->hThread == NULL)
    {
        printf("Failed to create engine thread!\n");
        engine->isRunning = 0;
        return;
    }

    MSG msg;

    while (engine->isRunning)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                engine->isRunning = 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(1); // Do not spin the main thread at 100% CPU just pumping
    }

    WaitForSingleObject(engine->hThread, INFINITE);
    CloseHandle(engine->hThread);
}

void DestroyEngine(EngineClass *engine)
{
    if (engine == NULL)
        return;

    engine->isRunning = 0;

    // IF the thread exists wait for it and release its handle

    if (engine->hThread != NULL && engine->hThread != INVALID_HANDLE_VALUE)
    {
        WaitForSingleObject(engine->hThread, INFINITE);
        CloseHandle(engine->hThread);
        engine->hThread = NULL;
    }

    if (engine->OnUserDestroyGame != NULL)
    {
        engine->OnUserDestroyGame(engine);
    }

    // Start Down here bellow add code for stopping allocations or threads IF you have em running for the game or systems stopping code

    // End code here

    // Releasing the globalWindowDeviceContext and Destorying the Game Window

    engine->engine_window = NULL;

    if (engine->hwnd != NULL)
    {
        if (engine->bitmap_pixel_buffer != NULL)
        {
            free(engine->bitmap_pixel_buffer);
            engine->bitmap_pixel_buffer = NULL;
        }

        if (engine->hdc != NULL)
        {
            ReleaseDC(engine->hwnd, engine->hdc);
            engine->hdc = NULL;
        }

        DestroyWindow(engine->hwnd);
        engine->hwnd = NULL;
    }
}