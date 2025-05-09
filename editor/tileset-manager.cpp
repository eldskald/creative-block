#include "tileset-manager.h"
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
    if (id >=
        (int)tileset_manager::tiles_.at(tileset_manager::selected_set).size())
        return -1;
    if (tileset_manager::get_tile_data(tileset_manager::selected_set, id)
            .type == tile_type::null)
        return -1;
    return id;
}

void tileset_manager::render_tile_(tileset set, int id) {
    spritesheet::render_sprite_at(
        tileset_manager::tiles_.at(set).at(id).spritesheet_coords,
        tileset_manager::get_tile_pos_(id));
}

// Initializing each tile means going through each tile on the spritesheet
// manually and setting here which ones are background, animations, blocks
// and interactables. We are no linting to avoid defining consts for this.
// NOLINTBEGIN
void tileset_manager::initialize() {
    tileset_manager::selected_tile = {{tileset::blocks, -1},
                                      {tileset::background, -1},
                                      {tileset::interact, -1}};

    vector<tile> block_tiles;
    for (int i = 0; i < 16; ++i) {
        block_tiles.push_back((tile){(Vector2){(float)i, 0}, tile_type::block});
        block_tiles.push_back((tile){(Vector2){(float)i, 1}, tile_type::block});
        block_tiles.push_back((tile){(Vector2){(float)i, 2}, tile_type::block});
        block_tiles.push_back((tile){(Vector2){(float)i, 3}, tile_type::block});
    }

    vector<tile> bg_tiles;
    // grass
    for (int i = 0; i < 4; ++i) {
        bg_tiles.push_back((tile){(Vector2){(float)i, 8}, tile_type::grass});
        bg_tiles.push_back((tile){(Vector2){(float)i, 10}, tile_type::grass});
    }
    // stones
    for (int i = 4; i < 6; ++i) {
        bg_tiles.push_back((tile){(Vector2){(float)i, 8}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 9}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 10}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 11}, tile_type::prop});
    }
    // mushrooms, bushes and trees
    for (int i = 0; i < 6; ++i) {
        bg_tiles.push_back((tile){(Vector2){(float)i, 12}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 13}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 14}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 15}, tile_type::prop});
    }
    // pipes, chains and drips
    for (int i = 6; i < 9; ++i) {
        bg_tiles.push_back((tile){(Vector2){(float)i, 8}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 9}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 10}, tile_type::prop});
        bg_tiles.push_back((tile){(Vector2){(float)i, 11}, tile_type::prop});
    }
    bg_tiles.push_back((tile){(Vector2){10, 8}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){10, 9}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){10, 10}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){11, 10}, tile_type::drip});
    bg_tiles.push_back((tile){(Vector2){11, 8}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){11, 9}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){13, 11}, tile_type::leaves});
    bg_tiles.push_back((tile){(Vector2){9, 15}, tile_type::puff});
    // waterfalls
    bg_tiles.push_back((tile){(Vector2){12, 8}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){12, 9}, tile_type::waterfall});
    bg_tiles.push_back((tile){(Vector2){14, 8}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){14, 9}, tile_type::waterfall});
    bg_tiles.push_back((tile){(Vector2){13, 8}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){13, 9}, tile_type::waterfall});
    bg_tiles.push_back((tile){(Vector2){15, 8}, tile_type::prop});
    bg_tiles.push_back((tile){(Vector2){15, 9}, tile_type::waterfall});
    // stars
    for (int i = 6; i < 9; ++i) {
        bg_tiles.push_back((tile){(Vector2){(float)i, 12}, tile_type::star});
    }
    for (int i = 6; i < 9; ++i) {
        bg_tiles.push_back((tile){(Vector2){(float)i, 14}, tile_type::prop});
    }

    vector<tile> int_tiles;
    int_tiles.push_back((tile){(Vector2){1, 5}, tile_type::player});
    int_tiles.push_back((tile){(Vector2){0, 4}, tile_type::goal});
    int_tiles.push_back((tile){(Vector2){7, 6}, tile_type::water});
    int_tiles.push_back((tile){(Vector2){10, 11}, tile_type::null});
    // spikes and platforms
    int_tiles.push_back((tile){(Vector2){3, 4}, tile_type::spike_t});
    int_tiles.push_back((tile){(Vector2){3, 5}, tile_type::spike_b});
    int_tiles.push_back((tile){(Vector2){3, 6}, tile_type::spike_l});
    int_tiles.push_back((tile){(Vector2){3, 7}, tile_type::spike_r});
    int_tiles.push_back((tile){(Vector2){4, 4}, tile_type::spike_v_t});
    int_tiles.push_back((tile){(Vector2){4, 5}, tile_type::spike_v_m});
    int_tiles.push_back((tile){(Vector2){4, 6}, tile_type::spike_v_b});
    int_tiles.push_back((tile){(Vector2){4, 7}, tile_type::platform});
    int_tiles.push_back((tile){(Vector2){5, 4}, tile_type::spike_h_l});
    int_tiles.push_back((tile){(Vector2){5, 5}, tile_type::spike_h_m});
    int_tiles.push_back((tile){(Vector2){5, 6}, tile_type::spike_h_r});
    int_tiles.push_back((tile){(Vector2){5, 7}, tile_type::platform});
    int_tiles.push_back((tile){(Vector2){0, 7}, tile_type::platform});
    int_tiles.push_back((tile){(Vector2){1, 7}, tile_type::platform});
    int_tiles.push_back((tile){(Vector2){2, 7}, tile_type::platform});
    int_tiles.push_back((tile){(Vector2){6, 7}, tile_type::platform});
    // gates
    int_tiles.push_back((tile){(Vector2){6, 4}, tile_type::button1});
    int_tiles.push_back((tile){(Vector2){6, 5}, tile_type::gate1});
    int_tiles.push_back((tile){(Vector2){6, 6}, tile_type::gate1});
    int_tiles.push_back((tile){(Vector2){10, 11}, tile_type::null});
    int_tiles.push_back((tile){(Vector2){6, 4}, tile_type::button2});
    int_tiles.push_back((tile){(Vector2){6, 5}, tile_type::gate2});
    int_tiles.push_back((tile){(Vector2){6, 6}, tile_type::gate2});
    int_tiles.push_back((tile){(Vector2){10, 11}, tile_type::null});
    int_tiles.push_back((tile){(Vector2){6, 4}, tile_type::button3});
    int_tiles.push_back((tile){(Vector2){6, 5}, tile_type::gate3});
    int_tiles.push_back((tile){(Vector2){6, 6}, tile_type::gate3});
    int_tiles.push_back((tile){(Vector2){10, 15}, tile_type::opening}); // id 31
    int_tiles.push_back((tile){(Vector2){10, 12}, tile_type::text1});   // id 32
    int_tiles.push_back((tile){(Vector2){10, 13}, tile_type::text2});   // id 33
    int_tiles.push_back((tile){(Vector2){10, 14}, tile_type::text3});   // id 34

    tileset_manager::tiles_ = {{tileset::blocks, block_tiles},
                               {tileset::background, bg_tiles},
                               {tileset::interact, int_tiles}};
}
// NOLINTEND

void tileset_manager::tick() {

    // Render the tiles
    for (int i = 0;
         i <
         (int)tileset_manager::tiles_.at(tileset_manager::selected_set).size();
         i++) {
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
