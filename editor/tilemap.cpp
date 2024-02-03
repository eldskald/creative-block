#include "tilemap.h"
#include "defs.h"
#include <array>
#include <raylib.h>
#include <stdexcept>

using namespace std;

void tilemap::set_tile(int x, int y, int tile_id) {
    this->cells_.at(x).at(y) = tile_id;
}

void tilemap::highlight_hovered_cell_(int x, int y) {
    if (x == -1 && y == -1) return;
    DrawRectangleLinesEx(
        (Rectangle){(float)(x * CELL_SIZE_X + TILEMAP_ORIGIN_X),
                    (float)(y * CELL_SIZE_Y + TILEMAP_ORIGIN_Y),
                    CELL_SIZE_X,
                    CELL_SIZE_Y},
        1,
        BLUE);
}

void tilemap::render_cell_(int x, int y) {
    switch (this->cells_.at(x).at(y)) {
    case 1:
        DrawTexturePro(defs::get_spritesheet(),
                       (Rectangle){SPRITESHEET_CELL_SIZE_X * 0,
                                   SPRITESHEET_CELL_SIZE_Y * 0,
                                   SPRITESHEET_CELL_SIZE_X,
                                   SPRITESHEET_CELL_SIZE_Y},
                       (Rectangle){(float)(x * CELL_SIZE_X + TILEMAP_ORIGIN_X),
                                   (float)(y * CELL_SIZE_Y + TILEMAP_ORIGIN_Y),
                                   CELL_SIZE_X,
                                   CELL_SIZE_Y},
                       (Vector2){0, 0},
                       0.0f,
                       WHITE);
        break;
    }
}

void tilemap::click_tile(int x, int y) {
    if (x == -1 && y == -1) return;
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
    this->set_tile(x, y, (this->cells_.at(x).at(y) + 1) % 2);
}

void tilemap::tick() {
    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();

    // Render tiles
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            this->render_cell_(i, j);
        }
    }

    // Make mouse equal to its cell coords or (-1, -1) if it's outside
    // the range of the tilemap
    if (mouse_x < TILEMAP_ORIGIN_X ||
        mouse_x > TILEMAP_ORIGIN_X + TILEMAP_PIXEL_SIZE_X ||
        mouse_y < TILEMAP_ORIGIN_Y ||
        mouse_y > TILEMAP_ORIGIN_Y + TILEMAP_PIXEL_SIZE_Y) {
        mouse_x = -1;
        mouse_y = -1;
    } else {
        mouse_x = (mouse_x - TILEMAP_ORIGIN_X) / CELL_SIZE_X;
        mouse_y = (mouse_y - TILEMAP_ORIGIN_Y) / CELL_SIZE_Y;
    }

    this->highlight_hovered_cell_(mouse_x, mouse_y);
    this->click_tile(mouse_x, mouse_y);
}
