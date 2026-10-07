#ifndef BITMAP_H
#define BITMAP_H
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t *CreatePixelBuffer(int width, int height);

void Create_BitMap(HWND hwnd, int Width, int Height);
void BitMap_Update(HDC hdc, HWND hwnd, uint32_t *bitmap_pixel_buffer, int width, int height);
void BitMap_Close(void);

#endif