#pragma once
#include <raylib.h>

#define EDITOR_FPS 60
#define LEVEL_SIZE_X 1280
#define LEVEL_SIZE_Y 720
#define EDITOR_WINDOW_SIZE_X (LEVEL_SIZE_X + 8)
#define EDITOR_WINDOW_SIZE_Y (LEVEL_SIZE_Y + 196)
#define EDITOR_WINDOW_TITLE "Level Editor"

#define SPRITESHEET_PATH "assets/spritesheet.png"
#define SPRITESHEET_CELL_X 8
#define SPRITESHEET_CELL_Y 8

#define CELL_SIZE_X 32
#define CELL_SIZE_Y 32

#define TILEMAP_ORIGIN_X 4
#define TILEMAP_ORIGIN_Y 4
#define TILEMAP_PIXEL_SIZE_X (LEVEL_SIZE_X)
#define TILEMAP_PIXEL_SIZE_Y (LEVEL_SIZE_Y)
#define TILEMAP_SIZE_X (LEVEL_SIZE_X / 4)
#define TILEMAP_SIZE_Y (LEVEL_SIZE_Y / 4)

#define TILESET_ORIGIN_X 4
#define TILESET_ORIGIN_Y (TILEMAP_ORIGIN_Y + TILEMAP_PIXEL_SIZE_Y + 8)
#define TILESET_ROWS 2

#define FONT_SIZE 20
#define TEXT_SPACING 2
#define INPUT_MAX_LENGTH 20
#define INPUT_MARGIN 4
#define INPUT_UNDERLINE_DIST 2
#define INPUT_HOLD_KEY_START 0.5f
#define INPUT_HOLD_KEY_PERIOD 0.05f
#define INPUT_BLINK_TIME 0.5f

#define POPUP_WIDTH 300
#define POPUP_HEIGHT 132
#define POPUP_PADDING 16
#define POPUP_LINE_SPACING 6
#define POPUP_BUTTON_WIDTH 100.0f
#define POPUP_BUTTON_HEIGHT 32.0f

#define FG_COLOR (Color){192, 202, 245, 255}                // #c0caf5
#define BG_COLOR (Color){26, 27, 38, 255}                   // #1a1b26
#define PRIMARY_COLOR (Color){65, 166, 181, 255}            // #41a6b5
#define HOVERED_COLOR (Color){115, 218, 202, 255}           // #73daca
#define FOCUSED_COLOR (Color){187, 154, 247, 255}           // #bb9af7
#define CELL_HIGHLIGHT_COLOR (Color){255, 255, 255, 32}
#define POPUP_BACKGROUND_SCREEN (Color){0, 0, 0, 128}

enum tile_type { background, block };

using tile = struct tile {
    Vector2 spritesheet_coords;
    tile_type type;
};
