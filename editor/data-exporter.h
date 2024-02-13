#pragma once
#include "defs.h"
#include "tilemap.h"
#include <array>
#include <raylib.h>
#include <string>

using namespace std;

class data_exporter {
public:
    static string get_export_text(map cells);

private:
    static Vector2* find_next_block_start_(map* cells);
    static string get_block_from_map_(Vector2 start, map* cells);
    static string get_physics_bodies_text_(map* cells);
    static string get_sprites_text_(map* cells);
};
