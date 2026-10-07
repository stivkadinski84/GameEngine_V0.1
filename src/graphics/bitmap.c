#include "graphics/bitmap.h"

BITMAPINFO bmi;

uint32_t *CreatePixelBuffer(int width, int height)
{
    uint32_t *pixel_buffer = malloc(width * height * sizeof(uint32_t));

    if (pixel_buffer == NULL)
    {
        return NULL;
    }

    return pixel_buffer;
}

void Create_BitMap(HWND hwnd, int Width, int Height)
{
    // HDC hdc = GetDC(hwnd);

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = Width;
    bmi.bmiHeader.biHeight = -Height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;
}

void BitMap_Update(
    HDC hdc,
    HWND hwnd,
    uint32_t *pixel_buffer,
    int width,
    int height)
{
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);

    StretchDIBits(
        hdc,
        0, 0,
        width, height,
        0, 0,
        width, height,
        pixel_buffer,
        &bmi,
        DIB_RGB_COLORS,
        SRCCOPY);
}

void Bitmap_Close(void)
{
}