#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H
#include "core/engine.h"
#include "core/sprite_engine.h"

typedef struct Sprite_Editor
{
    EngineClass engine;
    Sprite_Engine sprite_engine;
} Sprite_Editor;

int Sprite_Editor_OnUser_Create(EngineClass *engine);

int Sprite_Editor_OnUser_Update(EngineClass *engine, float fElapsedTime);

#endif