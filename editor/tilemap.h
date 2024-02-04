#pragma once
#include "defs.h"
#include <array>

using namespace std;

class tilemap {
public:
    tilemap();

    void set_tile(int x, int y, int tile_id);
    void tick();

private:
    array<array<int, TILEMAP_SIZE_Y>, TILEMAP_SIZE_X> cells_;

    void highlight_hovered_cell_(int x, int y);
    void render_cell_(int x, int y);
    void click_tile(int x, int y);
};
