#include "graphics/renderer.h"

Renderer CreateRenderer(uint32_t *renderingScreen, int width, int height)
{
    Renderer renderer;

    renderer.renderingSreen = renderingScreen;
    renderer.width = width;
    renderer.height = height;

    return renderer;
}

void Draw(Renderer *renderer, int x, int y, uint32_t color)
{
    if (x >= 0 && x < renderer->width && y >= 0 && y < renderer->height)
    {
        renderer->renderingSreen[y * renderer->width + x] = color;
    }
}