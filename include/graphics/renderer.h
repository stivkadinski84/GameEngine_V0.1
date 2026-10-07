#ifndef RENDERER_H
#define RENDERER_H
#include <stdint.h>

typedef struct Renderer
{
    uint32_t *renderingSreen;
    int width;
    int height;
} Renderer;

Renderer CreateRenderer(uint32_t *renderingSreen, int width, int height);

void Draw(Renderer *renderer, int x, int y, uint32_t color);

#endif