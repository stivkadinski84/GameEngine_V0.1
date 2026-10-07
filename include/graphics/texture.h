#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

// Texture Object or Structure Model known in C

// 1.The channels variable tells how much color information each pixel contains.
// common values are: 1 = grayscale, 3 = RGB, 4 = RGBA.

// 2.The pixels pointer variable stores the address of the texture's pixel data in memory
// each unsigned int can commonly hold a packed 32-bit color such as representing four 8-bit components.
typedef struct Texture
{
    int width;
    int height;
    int channels;
    unsigned int *pixels;
} Texture;

// Loading a texture function
Texture Load_Texture(const char *filepath);

// Freeing an allocated pixel memory
void Free_Texture(Texture *texture);
unsigned int Sample_Texture(Texture *texture, float u, float v);

#endif