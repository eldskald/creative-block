#pragma once
#include <raylib.h>

#define EDITOR_FPS 60
#define LEVEL_SIZE_X 1280
#define LEVEL_SIZE_Y 704
#define EDITOR_WINDOW_SIZE_X (LEVEL_SIZE_X + 8)
#define EDITOR_WINDOW_SIZE_Y (LEVEL_SIZE_Y + 232)
#define EDITOR_WINDOW_TITLE "Level Editor"

#define SPRITESHEET_PATH "assets/textures/spritesheet.png"
#define SPRITESHEET_CELL_X 8
#define SPRITESHEET_CELL_Y 8

#define CELL_SIZE_X 32
#define CELL_SIZE_Y 32

#define TILEMAP_ORIGIN_X 4
#define TILEMAP_ORIGIN_Y 4
#define TILEMAP_PIXEL_SIZE_X (LEVEL_SIZE_X)
#define TILEMAP_PIXEL_SIZE_Y (LEVEL_SIZE_Y)
#define TILEMAP_SIZE_X 40
#define TILEMAP_SIZE_Y 22

#define TILESET_ORIGIN_X 4
#define TILESET_ORIGIN_Y (TILEMAP_ORIGIN_Y + TILEMAP_PIXEL_SIZE_Y + 48)
#define TILESET_ROWS 4

#define FONT_SIZE 20
#define TEXT_SPACING 2
#define INPUT_MAX_LENGTH 50
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

#define FG_COLOR (Color){192, 202, 245, 255}      // #c0caf5
#define BG_COLOR (Color){26, 27, 38, 255}         // #1a1b26
#define PRIMARY_COLOR (Color){65, 166, 181, 255}  // #41a6b5
#define HOVERED_COLOR (Color){115, 218, 202, 255} // #73daca
#define FOCUSED_COLOR (Color){187, 154, 247, 255} // #bb9af7
#define CELL_HIGHLIGHT_COLOR (Color){255, 255, 255, 32}
#define POPUP_BACKGROUND_SCREEN (Color){0, 0, 0, 128}

enum tileset { blocks, background, interact };
enum tile_type {
    null,
    prop,
    grass,
    waterfall,
    star,
    water,
    drip,
    leaves,
    puff,
    block,
    platform,
    player,
    goal,
    spike_t,
    spike_b,
    spike_l,
    spike_r,
    spike_v_t,
    spike_v_m,
    spike_v_b,
    spike_h_l,
    spike_h_m,
    spike_h_r,
    button1,
    button2,
    button3,
    gate1,
    gate2,
    gate3,
    text1,
    text2,
    text3,
    opening,
    credits,
};

#define TILESETS {blocks, background, interact}
#define TILE_TYPES                                                             \
    {                                                                          \
        null,      prop,      grass,     waterfall, star,      water,          \
        drip,      leaves,    puff,      block,     platform,  player,         \
        goal,      spike_t,   spike_b,   spike_l,   spike_r,   spike_v_t,      \
        spike_v_m, spike_v_b, spike_h_l, spike_h_m, spike_h_r, button1,        \
        gate1,     button2,   gate2,     button3,   gate3,     text1,          \
        text2,     text3,     opening,   credits}

#define EXPORT_SPACE_CHAR '@'
#define PROJ_TEXT_SEPARATOR '@'

using tile = struct tile {
    Vector2 spritesheet_coords;
    tile_type type;
};
