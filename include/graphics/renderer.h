#ifndef RENDERER_H
#define RENDERER_H
#include <stdint.h>
#include "texture.h"

typedef struct Renderer
{
    uint32_t *renderingSreen;
    int width;
    int height;
} Renderer;

Renderer CreateRenderer(uint32_t *renderingSreen, int width, int height);

void Draw(Renderer *renderer, int x, int y, uint32_t color);

void DrawTexture(Renderer *renderer, int x, int y, Texture *texture);

#endif