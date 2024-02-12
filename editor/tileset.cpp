#include "tileset.h"
#include "defs.h"
#include "spritesheet.h"
#include <raylib.h>
#include <raymath.h>
#include <utility>
#include <vector>

using namespace std;

int tileset::selected_tile = -1;
vector<tile> tileset::tiles_;

tile tileset::get_tile_data(int id) {
    return tileset::tiles_.at(id);
}

Vector2 tileset::get_tile_sprite_coords(int id) {
    return tileset::tiles_.at(id).spritesheet_coords;
}

Vector2 tileset::get_tile_pos_(int id) {
    return (Vector2){
        (float)(TILESET_ORIGIN_X + CELL_SIZE_X * (id / TILESET_ROWS)),
        (float)(TILESET_ORIGIN_Y + CELL_SIZE_Y * (id % TILESET_ROWS))};
}

int tileset::get_mouse_tile_id_(Vector2 mouse_pos) {
    if (mouse_pos.x < TILESET_ORIGIN_X || mouse_pos.y < TILESET_ORIGIN_Y ||
        mouse_pos.y > TILESET_ORIGIN_Y + CELL_SIZE_Y * TILESET_ROWS) {
        return -1;
    }
    Vector2 offset = Vector2Subtract(
        mouse_pos, (Vector2){TILESET_ORIGIN_X, TILESET_ORIGIN_Y});
    int id = ((int)(offset.x) / CELL_SIZE_X) * TILESET_ROWS +
             (int)(offset.y) / CELL_SIZE_Y;
    return id < tileset::tiles_.size() ? id : -1;
}

void tileset::render_tile_(int id) {
    spritesheet::render_sprite_at(tileset::tiles_.at(id).spritesheet_coords,
                                  tileset::get_tile_pos_(id));
}

void tileset::initialize() {
    tileset::tiles_ = vector<tile>{(tile){(Vector2){0, 0}, block},
                                   (tile){(Vector2){1, 0}, block},
                                   (tile){(Vector2){2, 0}, block},
                                   (tile){(Vector2){3, 0}, block},
                                   (tile){(Vector2){0, 1}, block},
                                   (tile){(Vector2){1, 1}, block},
                                   (tile){(Vector2){2, 1}, block},
                                   (tile){(Vector2){3, 1}, block}};
}

void tileset::tick() {

    // Render the tiles
    for (int i = 0; i < tileset::tiles_.size(); i++) {
        tileset::render_tile_(i);
    }

    // Highlight hovered tile
    int hovered_id = tileset::get_mouse_tile_id_(
        (Vector2){(float)GetMouseX(), (float)GetMouseY()});
    if (hovered_id != -1) {
        Vector2 tile_origin = tileset::get_tile_pos_(hovered_id);
        DrawRectangle((int)tile_origin.x,
                      (int)tile_origin.y,
                      CELL_SIZE_X,
                      CELL_SIZE_Y,
                      CELL_HIGHLIGHT_COLOR);
    }

    // Highlight selected tile
    if (tileset::selected_tile != -1) {
        Vector2 tile_origin = tileset::get_tile_pos_(tileset::selected_tile);
        DrawRectangle((int)tile_origin.x,
                      (int)tile_origin.y,
                      CELL_SIZE_X,
                      CELL_SIZE_Y,
                      CELL_HIGHLIGHT_COLOR);
    }

    // Select tiles
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hovered_id != -1) {
        tileset::selected_tile =
            hovered_id == tileset::selected_tile ? -1 : hovered_id;
    }
}
