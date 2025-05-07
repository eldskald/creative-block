#pragma once
#include "tilemap.h"
#include <raylib.h>
#include <string>
#include <unordered_map>

using namespace std;

class data_exporter {
public:
    static string get_export_text(unordered_map<tileset, map> cells,
                                  string level_text_1,
                                  string level_text_2,
                                  string level_text_3);

private:
    static int current_id_count_;

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
    static string get_bg_puffs_text_(map* cells);
    static string get_player_text_(map* cells);
    static string get_spikes_text_(map* cells);
    static string get_level_text_1_text_(map* cells, string level_text);
    static string get_level_text_2_text_(map* cells, string level_text);
    static string get_level_text_3_text_(map* cells, string level_text);
};
