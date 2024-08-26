#pragma once
#include "tilemap.h"
#include <raylib.h>
#include <string>
#include <unordered_map>

using namespace std;

class data_exporter {
public:
    static string get_export_text(unordered_map<tileset, map> cells);

private:
    static Vector2* find_next_block_start_(map* cells);
    static string get_block_from_map_(Vector2 start, map* cells);
    static string get_physics_bodies_text_(map* cells);
    static string get_blocks_sprites_text_(map* cells);
    static string get_bg_props_text_(map* cells);
    static string get_bg_grass_text_(map* cells);
    static string get_bg_waterfall_text_(map* cells);
    static string get_bg_stars_text_(map* cells);
    static string get_bg_leaves_text_(map* cells);
    static string get_bg_drips_text_(map* cells);
    static string get_player_text_(map* cells);
};
