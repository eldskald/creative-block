#pragma once
#include "tilemap.h"
#include "tileset_manager.h"
#include <raylib.h>

class editor {
public:
    static bool mouse_disabled;

    static void initialize();
    static void tick();

private:
    static tilemap* tilemap_;
    static tileset_manager* tileset_manager_;

    static void change_to_blocks_tileset();
    static void change_to_bg_tileset();
    static void change_to_interact_tileset();

    static void save_tilemap_data();
    static void load_tilemap_data();
    static void export_tilemap_data();
};
