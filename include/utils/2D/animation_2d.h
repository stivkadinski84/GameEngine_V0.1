#ifndef ANIMATION_2D_H
#define ANIMATION_2D_H
#include "graphics/texture.h"

typedef struct AnimRectangle
{
    float x;
    float y;
    float width;
    float height;
} AnimRectangle;

typedef struct Animation_Frame
{
    // Sprite sheet is better for loading multiple sprites from single png sprite sheet
    Texture *spriteSheet;
    int frameWidth;
    int frameHeight;
    int columns;

    // Frame control is here
    int frameCount;
    int currentFrame;
    float frameDuration;
    float elapsedTime;
} Animation_Frame;

// Create Animation function here: Pass filepath, frame dimensions, total frames, and duration to initialize
Animation_Frame Animation_Create(Texture *sheet, int frameWidth, int frameHeight, int frameCount, float frameDuration);

// Update or Start loopin animation function here: Pass pointer to your animation struct and deltaTime from engine
void Animation_Update(Animation_Frame *animation, float deltaTime);

// Get the current animation frame or texture: Return the sub-rectangle (X, Y, W, H) on the sprite sheet for the active frame
AnimRectangle Animation_GetFrameRect(const Animation_Frame *animation);

// Reseting animation from the beggining sprite
void Animation_Reset(Animation_Frame *animation);

// Free/cleanup if necessary (or reset playback)
void Animation_Free(Animation_Frame *animation);

#endif