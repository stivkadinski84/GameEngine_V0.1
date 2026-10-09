#include <stdio.h>
#include "core/engine.h"

int OnCreateFunc(EngineClass *engine)
{
    return 1;
}

int OnUpdateFunc(EngineClass *engine, float fElapsedTime)
{
    Renderer *renderer = &engine->renderer;

    for (int y = 0; y < renderer->height; y++)
    {
        for (int x = 0; x < renderer->width; x++)
        {
            // Set the rendering screen to one color to start drawing on later so draw one color on it basically first
            Draw(renderer, x, y, FG_BLACK);
        }
    }
    return 1;
}

int main(int argc, char **argv)
{
    EngineClass engine = CreateEngine("Window Name", 800, 600);

    if (engine.hwnd == NULL)
        return 1;

    engine.OnUserCreateGame = OnCreateFunc;
    engine.OnUserUpdateGame = OnUpdateFunc;
    StartEngine(&engine);

    return 0;
}