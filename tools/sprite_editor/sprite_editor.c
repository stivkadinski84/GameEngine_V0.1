#include <stdio.h>
#include <stdlib.h>
#include "sprite_editor.h"
#include "ui/platform/windows/utils/side_panel.h"
#include "ui/platform/windows/utils/buttons.h"

int Sprite_Editor_OnUser_Create(EngineClass *engine)
{
    Sprite_Editor *sprite_editor = (Sprite_Editor *)engine;

    Create_SpriteEngine(&sprite_editor->sprite_engine, 32, 32);

    if (Create_SidePanel(engine->hwnd) == NULL)
    {
        return 0;
    }

    return 1;
}

int Sprite_Editor_OnUser_Update(EngineClass *engine, float fElapsedTime)
{
    Sprite_Editor *sprite_editor = (Sprite_Editor *)engine;

    Renderer *renderer = &engine->renderer;

    // Clear the whole screen first.
    for (int y = 0; y < renderer->height; y++)
    {
        for (int x = 0; x < renderer->width; x++)
        {
            Draw(renderer, x, y, 0xFF202020);
        }
    }

    // Draw the side panel over the background.
    // DrawSpriteEditorPanel(renderer);

    // Draw the button on the top of the panel
    // DrawButton(renderer, &openButton, 0xFF647B9B);

    return 1;
}

int main()
{
    Sprite_Editor sprite_editor = {0};

    sprite_editor.engine = CreateEngineForSpriteEditor("Sprite Editor", 1000, 800, 800, 600);

    sprite_editor.engine.OnUserCreateGame = Sprite_Editor_OnUser_Create;
    sprite_editor.engine.OnUserUpdateGame = Sprite_Editor_OnUser_Update;

    StartEngine(&sprite_editor.engine);
    return 0;
}