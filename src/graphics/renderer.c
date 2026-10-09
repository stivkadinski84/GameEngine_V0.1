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

void DrawTexture(Renderer *renderer, int x, int y, Texture *texture)
{
    // We start looping the texture pixels through it's height and width here
    for (int ty = 0; ty < texture->height; ty++)
    {
        for (int tx = 0; tx < texture->width; tx++)
        {

            // We create a 32 bit variable containing the colors of that texture into color format
            uint32_t color = texture->pixels[ty * texture->width + tx];

            // We create these variables because without them we would be painting every pixel at the same position all over
            // meaning we will placing one color at the same place all the time
            // so this is a Calculation of the current texture pixel where it should be placed on the rendering screen
            int screenX = x + tx;
            int screenY = y + ty;

            // And finalyl we use those variables to render the texture as a color format onto the rendering screen as a texture
            if (screenX >= 0 && screenX < renderer->width && screenY >= 0 && screenY < renderer->height)
            {
                // Extract the alpha byte from 0xAARRGGBB.
                uint32_t alpha = (color >> 24) & 0xFF;

                // Skip fully transparent pixels.
                if (alpha == 0)
                    continue;

                renderer->renderingSreen[screenY * renderer->width + screenX] = color;
            }
        }
    }
}