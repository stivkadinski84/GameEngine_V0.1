#include "utils/2D/animation_2d.h"

// Create Animation function here: Pass filepath, frame dimensions, total frames, and duration to initialize
Animation_Frame Animation_Create(Texture *sheet, int frameWidth, int frameHeight, int frameCount, float frameDuration)
{
    Animation_Frame animation = {0};

    animation.spriteSheet = sheet;
    animation.frameWidth = frameWidth;
    animation.frameHeight = frameHeight;
    animation.frameCount = frameCount;
    animation.frameDuration = frameDuration;
    animation.currentFrame = 0;
    animation.elapsedTime = 0.0f;

    if (sheet != NULL && frameWidth > 0)
    {
        animation.columns = sheet->width / frameWidth;
    }
    else
    {
        animation.columns = 1; // Fallback default
    }

    return animation;
}

// Update or Start loopin animation function here: Pass pointer to your animation struct and deltaTime from engine
void Animation_Update(Animation_Frame *animation, float deltaTime)
{
    if (animation->spriteSheet == NULL || animation->frameCount <= 0)
        return;

    animation->elapsedTime += deltaTime;

    if (animation->elapsedTime >= animation->frameDuration)
    {
        animation->elapsedTime -= animation->frameDuration;
        animation->currentFrame++;

        if (animation->currentFrame >= animation->frameCount)
        {
            animation->currentFrame = 0;
        }
    }
}

// Get the current animation frame or texture: Return the sub-rectangle (X, Y, W, H) on the sprite sheet for the active frame
AnimRectangle Animation_GetFrameRect(const Animation_Frame *animation)
{
    if (animation == NULL || animation->columns <= 0)
        return (AnimRectangle){0, 0, 0, 0};

    int gridColumnX = animation->currentFrame % animation->columns;
    int gridRowY = animation->currentFrame / animation->columns;

    return (AnimRectangle){
        (float)(gridColumnX * animation->frameWidth),
        (float)(gridRowY * animation->frameHeight),
        (float)animation->frameWidth,
        (float)animation->frameHeight};
}

// Use during gameplay to restart animation playback
void Animation_Reset(Animation_Frame *animation)
{
    if (animation == NULL)
        return;

    animation->currentFrame = 0;
    animation->elapsedTime = 0.0f;
}

// Free/cleanup if necessary (or reset playback)
void AnimationFree(Animation_Frame *animation)
{
    if (animation == NULL)
        return;

    animation->spriteSheet = NULL;
}
