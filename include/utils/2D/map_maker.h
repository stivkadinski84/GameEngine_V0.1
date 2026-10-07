#ifndef MAP_MAKER_H
#define MAP_MAKER_H
#include <stdint.h>
#include "texture.h"

// Map model change this or upgrade it if you need more complex map/level creation
typedef struct Map_Level
{
    int width;
    int height;
    const wchar_t **data;
    Texture material_texture;
} Map_Level;

// Draw the map here using symbols and connect the symbols with the right texture
// Example:
// const wchar_t *levelData[] =
//     {
//         L"..............................",
//         L"..............................",
//         L"..#..........###.............",
//         L".#.......##...................",
//         L".....###.....................",
//         L"..............................",
//         L"##############################"};

// Then create a model of that map/level to use it in your game
// Example:
// Map_Level level =
//     {
//         .width = 30,
//         .height = 7,
//         .data = levelData};



#endif