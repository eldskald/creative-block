#pragma once
#include <raylib.h>

const int EDITOR_FPS = 60;
const int EDITOR_WINDOW_SIZE_X = WINDOW_SIZE_X + 8;
const int EDITOR_WINDOW_SIZE_Y = WINDOW_SIZE_Y + 196;
const char* const EDITOR_WINDOW_TITLE = "Level Editor";

const int CELL_SIZE_X = SPRITESHEET_CELL_SIZE_X * PIXEL_SIZE_X;
const int CELL_SIZE_Y = SPRITESHEET_CELL_SIZE_Y * PIXEL_SIZE_Y;

const int TILEMAP_ORIGIN_X = 4;
const int TILEMAP_ORIGIN_Y = 4;
const int TILEMAP_PIXEL_SIZE_X = WINDOW_SIZE_X;
const int TILEMAP_PIXEL_SIZE_Y = WINDOW_SIZE_Y;
const int TILEMAP_SIZE_X = WINDOW_SIZE_X / CELL_SIZE_X;
const int TILEMAP_SIZE_Y = WINDOW_SIZE_Y / CELL_SIZE_Y;

const int TILESET_ORIGIN_X = 4;
const int TILESET_ORIGIN_Y = TILEMAP_ORIGIN_Y + TILEMAP_PIXEL_SIZE_Y + 8;
const int TILESET_ROWS = 2;

const int FONT_SIZE = 20;
const int TEXT_SPACING = 2;
const int INPUT_MARGIN = 4;
const int INPUT_UNDERLINE_DIST = 2;
const float INPUT_BLINK_TIME = 0.5;

const Color FG_COLOR = (Color){192, 202, 245, 255};      // #c0caf5
const Color BG_COLOR = (Color){26, 27, 38, 255};         // #1a1b26
const Color PRIMARY_COLOR = (Color){65, 166, 181, 255};  // #41a6b5
const Color HOVERED_COLOR = (Color){115, 218, 202, 255}; // #73daca
const Color FOCUSED_COLOR = (Color){187, 154, 247, 255}; // #bb9af7
const Color CELL_HIGHLIGHT_COLOR = (Color){255, 255, 255, 32};

enum tile_type { background, block };

using tile = struct tile {
    Vector2 spritesheet_coords;
    tile_type type;
};
