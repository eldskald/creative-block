#pragma once
#include "tilemap.h"
#include "tileset.h"
#include <raylib.h>

class editor {
public:
    static void initialize();
    static void tick();

private:
    static tilemap* tilemap_;
    static tileset* tileset_;

    static void save_tilemap_data();
    static void load_tilemap_data();
};
