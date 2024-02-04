#pragma once
#include "defs.h"
#include <raylib.h>
#include <vector>

using namespace std;

class tileset {
public:
    static int selected_tile;

    static void set_tiles(vector<tile> tiles);
    static Vector2 get_tile_sprite_coords(int id);

    static void initialize();
    static void tick();

private:
    static vector<tile> tiles_;

    static void render_tile_(int id);
    static int get_mouse_tile_id_(Vector2 mouse_pos);
    static Vector2 get_tile_pos_(int id);
};
