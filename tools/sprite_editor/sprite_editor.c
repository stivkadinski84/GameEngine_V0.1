#include <stdio.h>
#include <stdlib.h>
#include "sprite_editor.h"
#define PANEL_WIDTH 200

void DrawSpriteEditorPanel(Renderer *renderer)
{
    // Draw the left panel background.
    for (int y = 0; y < renderer->height; y++)
    {
        for (int x = 0; x < PANEL_WIDTH; x++)
        {
            Draw(renderer, x, y, 0xFF303030);
        }
    }
}

int Sprite_Editor_OnUser_Create(EngineClass *engine)
{
    Sprite_Editor *sprite_editor = (Sprite_Editor *)engine;

    Create_SpriteEngine(&sprite_editor->sprite_engine, 32, 32);
    
    return 1;
}

int Sprite_Editor_OnUser_Update(EngineClass *engine, float fElapsedTime)
{
    Sprite_Editor *sprite_editor = (Sprite_Editor *)engine;

    // Clear the whole screen first.
    Renderer *renderer = &engine->renderer;

    for (int y = 0; y < renderer->height; y++)
    {
        for (int x = 0; x < renderer->width; x++)
        {
            Draw(renderer, x, y, 0xFF202020);
        }
    }

    // Draw the side panel over the background.
    DrawSpriteEditorPanel(renderer);

    return 1;
}

int main()
{
    Sprite_Editor sprite_editor = {0};

    sprite_editor.engine = CreateEngine("Sprite Editor", 800, 600);

    sprite_editor.engine.OnUserCreateGame = Sprite_Editor_OnUser_Create;
    sprite_editor.engine.OnUserUpdateGame = Sprite_Editor_OnUser_Update;

    StartEngine(&sprite_editor.engine);
    return 0;
}