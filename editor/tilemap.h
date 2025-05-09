#pragma once
#include "defs.h"
#include "text-input.h"
#include <array>
#include <raylib.h>
#include <string>
#include <unordered_map>

using namespace std;

using map = array<array<int, TILEMAP_SIZE_Y>, TILEMAP_SIZE_X>;

class tilemap {
public:
    tilemap();

    void set_tile(tileset set, int x, int y, int tile_id);
    unordered_map<tileset, map> get_cells();
    string convert_to_data(string level_text_1,
                           string level_text_2,
                           string level_text_3);
    void load_from_data(string data,
                        text_input* input_1,
                        text_input* input_2,
                        text_input* input_3);
    void tick();

private:
    unordered_map<tileset, map> cells_;
    int text_1_x_{-1};
    int text_1_y_{-1};
    int text_2_x_{-1};
    int text_2_y_{-1};
    int text_3_x_{-1};
    int text_3_y_{-1};
    int opening_x_{-1};
    int opening_y_{-1};

    void highlight_hovered_cell_(int x, int y);
    void render_cell_(int x, int y);
    void click_tile(int x, int y);
};
