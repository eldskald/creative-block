#include "tileset_manager.h"
#include "defs.h"
#include "editor.h"
#include "spritesheet.h"
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <vector>

using namespace std;

unordered_map<tileset, int> tileset_manager::selected_tile;
tileset tileset_manager::selected_set = tileset::blocks;
unordered_map<tileset, vector<tile>> tileset_manager::tiles_;

tile tileset_manager::get_tile_data(tileset set, int id) {
    return tileset_manager::tiles_.at(set).at(id);
}

Vector2 tileset_manager::get_tile_sprite_coords(tileset set, int id) {
    return tileset_manager::tiles_.at(set).at(id).spritesheet_coords;
}

Vector2 tileset_manager::get_tile_pos_(int id) {
    return (Vector2){
        (float)(TILESET_ORIGIN_X + CELL_SIZE_X * (id / TILESET_ROWS)),
        (float)(TILESET_ORIGIN_Y + CELL_SIZE_Y * (id % TILESET_ROWS))};
}

int tileset_manager::get_mouse_tile_id_(Vector2 mouse_pos) {
    if (mouse_pos.x < TILESET_ORIGIN_X || mouse_pos.y < TILESET_ORIGIN_Y ||
        mouse_pos.y > TILESET_ORIGIN_Y + CELL_SIZE_Y * TILESET_ROWS) {
        return -1;
    }
    Vector2 offset = Vector2Subtract(
        mouse_pos, (Vector2){TILESET_ORIGIN_X, TILESET_ORIGIN_Y});
    int id = ((int)(offset.x) / CELL_SIZE_X) * TILESET_ROWS +
             (int)(offset.y) / CELL_SIZE_Y;
    return id < tileset_manager::tiles_.size() ? id : -1;
}

void tileset_manager::render_tile_(tileset set, int id) {
    spritesheet::render_sprite_at(
        tileset_manager::tiles_.at(set).at(id).spritesheet_coords,
        tileset_manager::get_tile_pos_(id));
}

void tileset_manager::initialize() {
    tileset_manager::selected_tile = {{tileset::blocks, -1},
                                      {tileset::background, -1},
                                      {tileset::interact, -1}};

    auto block_tiles = vector<tile>{(tile){(Vector2){0, 0}, tile_type::block},
                                    (tile){(Vector2){1, 0}, tile_type::block},
                                    (tile){(Vector2){2, 0}, tile_type::block},
                                    (tile){(Vector2){3, 0}, tile_type::block},
                                    (tile){(Vector2){0, 1}, tile_type::block},
                                    (tile){(Vector2){1, 1}, tile_type::block},
                                    (tile){(Vector2){2, 1}, tile_type::block},
                                    (tile){(Vector2){3, 1}, tile_type::block}};
    auto bg_tiles = vector<tile>{(tile){(Vector2){0, 0}, tile_type::prop},
                                 (tile){(Vector2){0, 1}, tile_type::prop}};
    auto interact_tiles =
        vector<tile>{(tile){(Vector2){0, 0}, tile_type::player},
                     (tile){(Vector2){0, 1}, tile_type::goal}};
    tileset_manager::tiles_ = {{tileset::blocks, block_tiles},
                               {tileset::background, bg_tiles},
                               {tileset::interact, interact_tiles}};
}

void tileset_manager::tick() {

    // Render the tiles
    for (int i = 0; i < tileset_manager::tiles_.size(); i++) {
        tileset_manager::render_tile_(tileset_manager::selected_set, i);
    }

    // Highlight selected tile
    if (tileset_manager::selected_tile.at(tileset_manager::selected_set) !=
        -1) {
        Vector2 tile_origin = tileset_manager::get_tile_pos_(
            tileset_manager::selected_tile.at(tileset_manager::selected_set));
        DrawRectangle((int)tile_origin.x,
                      (int)tile_origin.y,
                      CELL_SIZE_X,
                      CELL_SIZE_Y,
                      CELL_HIGHLIGHT_COLOR);
    }

    if (editor::mouse_disabled) return;

    // Highlight hovered tile
    int hovered_id = tileset_manager::get_mouse_tile_id_(
        (Vector2){(float)GetMouseX(), (float)GetMouseY()});
    if (hovered_id != -1) {
        Vector2 tile_origin = tileset_manager::get_tile_pos_(hovered_id);
        DrawRectangle((int)tile_origin.x,
                      (int)tile_origin.y,
                      CELL_SIZE_X,
                      CELL_SIZE_Y,
                      CELL_HIGHLIGHT_COLOR);
    }

    // Select tiles
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hovered_id != -1) {
        tileset_manager::selected_tile.at(tileset_manager::selected_set) =
            hovered_id == tileset_manager::selected_tile.at(
                              tileset_manager::selected_set)
                ? -1
                : hovered_id;
    }
}
