#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#include <Windows.h>

typedef struct
{
    int bPressed;
    int bHeld;
    int bReleased;
} KeyState;

typedef struct
{
    // 256 Virtual Key states (covers keyboard + mouse buttons VK_LBUTTON/VK_RBUTTON)
    KeyState keys[256];
    short oldKeys[256];
    short newKeys[256];

    // Mouse positions mapped directly into the map grid cells
    int mouseX;
    int mouseY;

    // Mouse button states matching the structure
    KeyState mouse[5];
    int oldMouseButtons[5];
    int newMouseButtons[5];
    int inFocus;
} Input_System;

// Forward declaration to avoid circular includes and circular dependencies "This type/function exists. You don't need to know its full definition yet."
typedef struct EngineClass EngineClass;

// Fully processes keyboard state structures and console mouse event queues
void Update_Input(Input_System *input, EngineClass *engine);

#endif