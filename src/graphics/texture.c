#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "graphics/stb_image.h"
#include "graphics/texture.h"

Texture Load_Texture(const char *filepath)
{
    // Creating an empty texture
    Texture texture = {0};

    // Creating a raw pointer for image data
    unsigned char *data = NULL;

    // Storing and Loading the image's width, height, channels in the texture as RGBA (4 channels)
    data = stbi_load(filepath, &texture.width, &texture.height, &texture.channels, 4);

    // Error checking if the image is loaded
    if (data == NULL)
    {
        printf("Texture loading has failed check texture.h texture.c files");
        return texture;
    }

    // We are forcing 4 channels: R, G, B, A
    texture.channels = 4;

    // Allocating image pixels array in memory
    texture.pixels = (unsigned int *)malloc(sizeof(unsigned int) * texture.width * texture.height);

    // Checking if allocation of pixels failed or succeeded
    if (texture.pixels == NULL)
    {
        printf("Texture pixel allocation failed! Check texture.c file!\n");

        stbi_image_free(data);

        texture.width = 0;
        texture.height = 0;

        return texture;
    }

    // Going through every pixel or traversing the array of thr raw image data
    for (int i = 0; i < texture.width * texture.height; i++)
    {
        // Collecting individual pixel color information into 8-bit variables [R, G, B, A]
        unsigned char r = data[i * 4 + 0];
        unsigned char g = data[i * 4 + 1];
        unsigned char b = data[i * 4 + 2];
        unsigned char a = data[i * 4 + 3];

        // Converting them into a single 32-bit integer (0x00RRGGBB) currently nto storing the alpha value add it later if you want
        // texture.pixels[i] = (a == 0) ? 0x000000 : (r << 16) | (g << 8) | b;

        texture.pixels[i] =
            ((unsigned int)a << 24) |
            ((unsigned int)r << 16) |
            ((unsigned int)g << 8) |
            (unsigned int)b;
    }

    // Free stb's temporary byte array memory
    stbi_image_free(data);

    // Finally returning the texture if everyting is correct
    return texture;
}

void Free_Texture(Texture *texture)
{
    // Checking if the pixels are not NULL or contain colors
    if (texture->pixels != NULL)
    {
        // Freeing the pixels memory allocated and setting the texture pixels pointer to nothing or NULL
        free(texture->pixels);
        texture->pixels = NULL;

        texture->width = 0;
        texture->height = 0;
        texture->channels = 0;
    }
}

// 3D TEXTURE SCALING / RAYCASTING (UV SAMPLING)
unsigned int Sample_Texture(Texture *texture, float u, float v)
{

    // Checking if the pixels are not null if they are NULL set them to Black or any color that your game window/renderer portrays as background color
    if (texture->pixels == NULL)
        return 0x000000;

    // Convert u into an x pixel coordinate
    int sx = (int)(u * (float)texture->width);

    // Convert v into a y pixel coordinate
    int sy = (int)(v * (float)texture->height);

    // Make sure x isn't outside the texture
    if (sx < 0)
        sx = 0;
    if (sx >= texture->width)
        sx = texture->width - 1;

    // Make sure y isn't outside the texture
    if (sy < 0)
        sy = 0;
    if (sy >= texture->height)
        sy = texture->height - 1;

    // Return that texture pixel's color
    return texture->pixels[sy * texture->width + sx];
}