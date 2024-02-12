#pragma once
#include "defs.h"
#include <array>
#include <string>

using namespace std;

using map = array<array<int, TILEMAP_SIZE_Y>, TILEMAP_SIZE_X>;

class tilemap {
public:
    tilemap();

    void set_tile(int x, int y, int tile_id);
    map get_cells();
    string convert_to_data();
    void load_from_data(string data);
    void tick();

private:
    map cells_;

    void highlight_hovered_cell_(int x, int y);
    void render_cell_(int x, int y);
    void click_tile(int x, int y);
};
