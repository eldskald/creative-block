#pragma once
#include <raylib.h>

const int EDITOR_FPS = 60;
const int EDITOR_WINDOW_SIZE_X = WINDOW_SIZE_X + 8;
const int EDITOR_WINDOW_SIZE_Y = WINDOW_SIZE_Y + 196;
const Color EDITOR_BG_COLOR = (Color){26, 27, 38, 255};

const int TILEMAP_ORIGIN_X = 4;
const int TILEMAP_ORIGIN_Y = 4;
const int TILEMAP_PIXEL_SIZE_X = WINDOW_SIZE_X;
const int TILEMAP_PIXEL_SIZE_Y = WINDOW_SIZE_Y;

const int CELL_SIZE_X = SPRITESHEET_CELL_SIZE_X * PIXEL_SIZE_X;
const int CELL_SIZE_Y = SPRITESHEET_CELL_SIZE_Y * PIXEL_SIZE_Y;
const int TILEMAP_SIZE_X = WINDOW_SIZE_X / CELL_SIZE_X;
const int TILEMAP_SIZE_Y = WINDOW_SIZE_Y / CELL_SIZE_Y;

class defs {
public:
    static void initialize();
    static Texture2D get_spritesheet();

private:
    static Texture2D atlas_;
};
