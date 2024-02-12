#include "data-exporter.h"
#include "defs.h"
#include "tilemap.h"
#include "tileset.h"
#include <array>
#include <iostream>
#include <raylib.h>
#include <string>

Vector2* data_exporter::find_next_block_start_(map* cells) {
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            if (cells->at(i).at(j) == -1) {
                continue;
            } else if (tileset::get_tile_data(cells->at(i).at(j)).type ==
                       tile_type::block) {
                auto start = new Vector2();
                start->x = (float)i;
                start->y = (float)j;
                return start;
            }
        }
    }
    return nullptr;
}

string data_exporter::get_block_from_map_(Vector2 start, map* cells) {

    // Initiate block position and shape
    Rectangle rect = (Rectangle){start.x, start.y, 1.0, 0.0};

    // Let's find the block height
    int coords_x = (int)start.x;
    int coords_y = (int)start.y;
    while (true) {
        if (cells->at(coords_x).at(coords_y) == -1) {
            break;
        } else if (tileset::get_tile_data(cells->at(coords_x).at(coords_y))
                       .type != tile_type::block) {
            break;
        } else {
            cells->at(coords_x).at(coords_y) = -1;
            rect.height += 1.0;
            coords_y++;
            if (coords_y == TILEMAP_SIZE_Y) break;
        }
    }

    // Now we check if the whole column at the right of the one we just removed
    // is also composed of solid blocks. If so, we remove them from the map and
    // add 1 to rect width. Then we do it again until we find one that's not
    // entirely made of solid blocks.
    coords_y = (int)start.y;
    while (true) {
        coords_x++;
        if (coords_x > TILEMAP_SIZE_X) break;

        // Checks if the next column is solid
        bool is_column_solid_blocks = true;
        for (int i = 0; i < (int)rect.height; i++) {
            if (cells->at(coords_x).at(coords_y + i) == -1) {
                is_column_solid_blocks = false;
                break;
            } else if (tileset::get_tile_data(cells->at(coords_x).at(coords_y))
                           .type != tile_type::block) {
                is_column_solid_blocks = false;
                break;
            }
        }

        // Breaks if it isn't
        if (!is_column_solid_blocks) break;

        // If it is, increase block width and remove the column from the map
        rect.width += 1.0;
        for (int i = 0; i < (int)rect.height; i++) {
            cells->at(coords_x).at(coords_y + i) = -1;
        }
    }

    // We've been treating the rect in cell unit lengths. We should transform
    // them to pixel before exporting data.
    rect.x *= CELL_SIZE_X;
    rect.y *= CELL_SIZE_Y;
    rect.width *= CELL_SIZE_X;
    rect.height *= CELL_SIZE_Y;

    // Make the return string based on the calculated rect
    string str = "[physics_body]\n";
    str += "pos = (" + to_string((int)rect.x) + "," + to_string((int)rect.y) +
           ")\n";
    str += "type = fixed\n";
    str += "collision_box = (0,0," + to_string((int)rect.width) + "," +
           to_string((int)rect.height) + ")\n";
    str += "collision_layer = 00000100\n";
    return str;
}

string data_exporter::get_export_text(map cells) {
    string text = "";
    Vector2* start = data_exporter::find_next_block_start_(&cells);
    while (start) {
        text += data_exporter::get_block_from_map_(*start, &cells);
        text += '\n';
        delete start;
        start = data_exporter::find_next_block_start_(&cells);
    }
    return text;
}
