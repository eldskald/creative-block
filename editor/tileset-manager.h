#pragma once
#include "defs.h"
#include <raylib.h>
#include <unordered_map>
#include <vector>

using namespace std;

class tileset_manager {
public:
    static unordered_map<tileset, int> selected_tile;
    static tileset selected_set;

    static tile get_tile_data(tileset set, int id);
    static Vector2 get_tile_sprite_coords(tileset set, int id);

    static void initialize();
    static void tick();

private:
    static unordered_map<tileset, vector<tile>> tiles_;

    static void render_tile_(tileset set, int id);
    static int get_mouse_tile_id_(Vector2 mouse_pos);
    static Vector2 get_tile_pos_(int id);
};
