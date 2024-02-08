#pragma once
#include "defs.h"
#include <array>
#include <string>

using namespace std;

class tilemap {
public:
    tilemap();

    void set_tile(int x, int y, int tile_id);
    string convert_to_data();
    void load_from_data(string data);
    void tick();

private:
    array<array<int, TILEMAP_SIZE_Y>, TILEMAP_SIZE_X> cells_;

    void highlight_hovered_cell_(int x, int y);
    void render_cell_(int x, int y);
    void click_tile(int x, int y);
};
