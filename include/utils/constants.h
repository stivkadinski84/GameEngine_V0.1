#ifndef CONSTANTS_H
#define CONSTANTS_H

// Foreground Truecolor (24-bit RGB: 0xRRGGBB)
enum COLOUR
{
    FG_BLACK        = 0x000000,
    FG_DARK_BLUE    = 0x000080,
    FG_DARK_GREEN   = 0x008000,
    FG_DARK_CYAN    = 0x008080,
    FG_DARK_RED     = 0x800000,
    FG_DARK_MAGENTA = 0x800080,
    FG_DARK_YELLOW  = 0x808000,
    FG_GREY         = 0xC0C0C0,
    FG_DARK_GREY    = 0x808080,
    FG_BLUE         = 0x0000FF,
    FG_GREEN        = 0x00FF00,
    FG_CYAN         = 0x00FFFF,
    FG_RED          = 0xFF0000,
    FG_MAGENTA      = 0xFF00FF,
    FG_YELLOW       = 0xFFFF00,
    FG_WHITE        = 0xFFFFFF,

    // Background uses the same RGB values in a pixel/RGB renderer.
    BG_BLACK        = 0x000000,
    BG_DARK_BLUE    = 0x000080,
    BG_DARK_GREEN   = 0x008000,
    BG_DARK_CYAN    = 0x008080,
    BG_DARK_RED     = 0x800000,
    BG_DARK_MAGENTA = 0x800080,
    BG_DARK_YELLOW  = 0x808000,
    BG_GREY         = 0xC0C0C0,
    BG_DARK_GREY    = 0x808080,
    BG_BLUE         = 0x0000FF,
    BG_GREEN        = 0x00FF00,
    BG_CYAN         = 0x00FFFF,
    BG_RED          = 0xFF0000,
    BG_MAGENTA      = 0xFF00FF,
    BG_YELLOW       = 0xFFFF00,
    BG_WHITE        = 0xFFFFFF,
};

// Shading glyphs are no longer needed for RGB pixel rendering.
// Dithering/shading is handled by directly blending RGB values.
enum PIXEL_TYPE
{
    PIXEL_SOLID         = 100, // 100% color intensity
    PIXEL_THREEQUARTERS = 75,  //  75% color intensity
    PIXEL_HALF          = 50,  //  50% color intensity
    PIXEL_QUARTER       = 25,  //  25% color intensity
};

#endif