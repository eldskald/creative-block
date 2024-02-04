#include "tilemap.h"
#include "defs.h"
#include "spritesheet.h"
#include "tileset.h"
#include <array>
#include <raylib.h>
#include <stdexcept>

using namespace std;

tilemap::tilemap() { // NOLINT
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            this->cells_.at(i).at(j) = -1;
        }
    }
}

void tilemap::set_tile(int x, int y, int tile_id) {
    this->cells_.at(x).at(y) = tile_id;
}

void tilemap::highlight_hovered_cell_(int x, int y) {
    if (x == -1 && y == -1) return;
    DrawRectangle(x * CELL_SIZE_X + TILEMAP_ORIGIN_X,
                  y * CELL_SIZE_Y + TILEMAP_ORIGIN_Y,
                  CELL_SIZE_X,
                  CELL_SIZE_Y,
                  CELL_HIGHLIGHT_COLOR);
}

void tilemap::render_cell_(int x, int y) {
    int id = this->cells_.at(x).at(y);
    if (id == -1) return;

    Vector2 coords = tileset::get_tile_sprite_coords(id);
    spritesheet::render_sprite_at(
        coords,
        (Vector2){(float)(x * CELL_SIZE_X + TILEMAP_ORIGIN_X),
                  (float)(y * CELL_SIZE_Y + TILEMAP_ORIGIN_Y)});
}

void tilemap::click_tile(int x, int y) {
    if (x == -1 && y == -1) return;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        this->set_tile(x, y, tileset::selected_tile);
    } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        this->set_tile(x, y, -1);
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
        tileset::selected_tile = this->cells_.at(x).at(y);
    }
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
