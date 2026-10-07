#ifndef ENGINE_H
#define ENGINE_H
#include <Windows.h>
#include "graphics/renderer.h"
#include "core/thread.h"
#include "utils/constants.h"
#include "input/input_system.h"
#include "utils/2D/animation_2d.h"

typedef struct EngineClass EngineClass;

typedef int (*UserCreateProc)(EngineClass *engine);
typedef int (*UserUpdateProc)(EngineClass *engine, float fElapsedTime);
typedef int (*UserDestroyProc)(EngineClass *engine);

typedef struct EngineClass
{
    // Systems like renderer audio engine and input engine
    Renderer renderer;
    Input_System input;

    // Windows specific handles needed for the thread and the bitmap
    HWND hwnd;
    HDC hdc;
    uint32_t *engine_window;
    uint32_t *bitmap_pixel_buffer;

    // Threading functions
    HANDLE hThread;
    // boolean for returning check if the game is running ot not giving us perspective whether to kill thread and game process
    int isRunning;

    // Main functions from the game engine class to create the game objectives update (meaning as framerate flows update changes to screen) and destroy the game or kill the game
    UserCreateProc OnUserCreateGame;
    UserUpdateProc OnUserUpdateGame;
    UserDestroyProc OnUserDestroyGame;
} EngineClass;

EngineClass CreateEngine(const char *title, int width, int height);
void StartEngine(EngineClass *engine);
void DestroyEngine(EngineClass *engine);

#endif