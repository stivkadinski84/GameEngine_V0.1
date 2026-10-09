#include <stdio.h>
#include <stdlib.h>
#include "core/sprite_engine.h"

void Create_SpriteEngine(Sprite_Engine *sprite_engine, int w, int h)
{
    sprite_engine->width = w;
    sprite_engine->height = h;
    sprite_engine->Pixels = malloc(sizeof(short) * w * h);
    sprite_engine->Colors = malloc(sizeof(short) * w * h);

    for (int i = 0; i < w * h; i++)
    {
        sprite_engine->Pixels[i] = L' ';
        sprite_engine->Colors[i] = FG_BLACK;
    }
}