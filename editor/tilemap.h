#pragma once
#include "defs.h"
#include <array>
#include <string>
#include <unordered_map>
#include <raylib.h>

using namespace std;

using map = array<array<int, TILEMAP_SIZE_Y>, TILEMAP_SIZE_X>;

class tilemap {
public:
    tilemap();

    void set_tile(tileset set, int x, int y, int tile_id);
    unordered_map<tileset, map> get_cells();
    string convert_to_data(string level_text);
    string load_from_data(string data);
    void tick();

private:
    unordered_map<tileset, map> cells_;
    int text_x_{-1};
    int text_y_{-1};

    void highlight_hovered_cell_(int x, int y);
    void render_cell_(int x, int y);
    void click_tile(int x, int y);
};
