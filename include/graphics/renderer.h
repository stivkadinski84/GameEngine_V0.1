#ifndef RENDERER_H
#define RENDERER_H
#include <stdint.h>
#include "texture.h"
#include "tools/ui/platform/windows/utils/buttons.h"
#include "tools/ui/platform/windows/utils/side_panel.h"

typedef struct Renderer
{
    uint32_t *renderingSreen;
    int width;
    int height;
} Renderer;

Renderer CreateRenderer(uint32_t *renderingSreen, int width, int height);

void Draw(Renderer *renderer, int x, int y, uint32_t color);

void DrawTexture(Renderer *renderer, int x, int y, Texture *texture);

// BUTTONS DRAWING ROUTINES
// void DrawButton(Renderer *renderer, const ButtonType1 *button1, uint32_t color);

#endif