#ifndef SPRITE_ENGINE_H
#define SPRITE_ENGINE_H
#include "utils/constants.h"

typedef struct Sprite_Engine
{
    int width;
    int height;
    short *Pixels;
    short *Colors;
    short (*pfnGetPixel)(struct Sprite_Engine *sprite_engine, int x, int y);
    short (*pfnGetColor)(struct Sprite_Engine *sprite_engine, int x, int y);
    short (*pfnSetPixel)(struct Sprite_Engine *sprite_engine, int x, int y, short color);
} Sprite_Engine;

void Create_SpriteEngine(Sprite_Engine *sprite_engine, int w, int h);
int Load_Sprite(Sprite_Engine *sprite_engine, const wchar_t *file);

#endif