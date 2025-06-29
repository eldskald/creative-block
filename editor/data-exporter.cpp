#include "data-exporter.h"
#include "defs.h"
#include "tilemap.h"
#include "tileset-manager.h"
#include <array>
#include <raylib.h>
#include <string>
#include <unordered_map>

using namespace std;

int data_exporter::current_id_count_ = 0;

string data_exporter::get_out_of_bounds_walls_() {
    string r = "[physics_body]\n";
    r += "pos = (-" + to_string(SPRITESHEET_CELL_X) + ",0)\n";
    r += "type = fixed\n";
    r += "collision_box = (0,0," + to_string(SPRITESHEET_CELL_X) + "," +
         to_string(GAME_SIZE_Y) + ")\n";
    r += "collision_layer = 00000100\n\n";
    r += "[physics_body]\n";
    r += "pos = (0,-" + to_string(SPRITESHEET_CELL_Y) + ")\n";
    r += "type = fixed\n";
    r += "collision_box = (0,0," + to_string(GAME_SIZE_X) + "," +
         to_string(SPRITESHEET_CELL_Y) + ")\n";
    r += "collision_layer = 00000100\n\n";
    r += "[physics_body]\n";
    r += "pos = (" + to_string(GAME_SIZE_X) + ",0)\n";
    r += "type = fixed\n";
    r += "collision_box = (0,0," + to_string(SPRITESHEET_CELL_X) + "," +
         to_string(GAME_SIZE_Y) + ")\n";
    r += "collision_layer = 00000100\n\n";
    r += "[physics_body]\n";
    r += "pos = (0," + to_string(GAME_SIZE_Y) + ")\n";
    r += "type = fixed\n";
    r += "collision_box = (0,0," + to_string(GAME_SIZE_X) + "," +
         to_string(SPRITESHEET_CELL_Y) + ")\n";
    r += "collision_layer = 00000100\n\n";
    return r;
}

Vector2* data_exporter::find_next_block_start_(map* cells) {
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            if (cells->at(i).at(j) == -1) {
                continue;
            } else {
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
    Rectangle rect = (Rectangle){start.x, start.y, 1.0f, 0.0f};

    // Let's find the block height
    int coords_x = (int)start.x;
    int coords_y = (int)start.y;
    while (true) {
        if (cells->at(coords_x).at(coords_y) == -1) {
            break;
        } else {
            cells->at(coords_x).at(coords_y) = -1;
            rect.height += 1.0f;
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
        if (coords_x >= TILEMAP_SIZE_X) break;

        // Checks if the next column is solid
        bool is_column_solid_blocks = true;
        for (int i = 0; i < (int)rect.height; i++) {
            if (cells->at(coords_x).at(coords_y + i) == -1) {
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
    rect.x *= SPRITESHEET_CELL_X;
    rect.y *= SPRITESHEET_CELL_Y;
    rect.width *= SPRITESHEET_CELL_X;
    rect.height *= SPRITESHEET_CELL_Y;

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

string data_exporter::get_physics_bodies_text_(map* cells) {
    string text = "";
    Vector2* start = data_exporter::find_next_block_start_(cells);
    while (start) {
        text += data_exporter::get_block_from_map_(*start, cells);
        text += '\n';
        delete start;
        start = data_exporter::find_next_block_start_(cells);
    }
    return text;
}

string data_exporter::get_blocks_sprites_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            if (cells->at(i).at(j) == -1) continue;
            Vector2 coords = tileset_manager::get_tile_sprite_coords(
                tileset::blocks, cells->at(i).at(j));
            text += "[sprite]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "atlas_coords = (" + to_string((int)coords.x) + "," +
                    to_string((int)coords.y) + ")\n";
            text += "tint = (255,0,0,255)\n";
            text += "\n";
        }
    }
    return text;
}

Vector2* data_exporter::find_next_platform_start_(map* cells) {
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::platform) continue;
            auto start = new Vector2();
            start->x = (float)i;
            start->y = (float)j;
            return start;
        }
    }
    return nullptr;
}

string data_exporter::get_platform_from_map_(Vector2 start, map* cells) {
    int length = 0;
    int coords_x = (int)start.x;
    while (true) {
        int cell_id = cells->at(coords_x).at((int)start.y);
        if (cell_id == -1) break;
        tile data = tileset_manager::get_tile_data(tileset::interact, cell_id);
        if (data.type != tile_type::platform) break;
        cells->at(coords_x).at((int)start.y) = -1;
        length++;
        coords_x++;
        if (coords_x == TILEMAP_SIZE_X) break;
    }
    string str = "[physics_body]\n";
    str += "pos = (" + to_string((int)start.x * SPRITESHEET_CELL_X) + "," +
           to_string((int)start.y * SPRITESHEET_CELL_Y) + ")\n";
    str += "type = fixed\n";
    str += "one_way = true\n";
    str += "collision_box = (0,0," + to_string(length * SPRITESHEET_CELL_X) +
           "," + to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
    str += "collision_layer = 00001000\n";
    return str;
}

string data_exporter::get_platform_bodies_text_(map* cells) {
    string text = "";
    Vector2* start = data_exporter::find_next_platform_start_(cells);
    while (start) {
        text += data_exporter::get_platform_from_map_(*start, cells);
        text += "\n";
        delete start;
        start = data_exporter::find_next_platform_start_(cells);
    }
    return text;
}

string data_exporter::get_platform_sprites_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::platform) continue;

            Vector2 coords = tileset_manager::get_tile_sprite_coords(
                tileset::interact, cell_id);
            text += "[sprite]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "atlas_coords = (" + to_string((int)coords.x) + "," +
                    to_string((int)coords.y) + ")\n";
            text += "tint = (255,0,0,255)\n";
            text += "\n";
        }
    }
    return text;
}

Vector2* data_exporter::find_next_shadow_platform_start_(map* cells) {
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::shadow_platform) continue;
            auto start = new Vector2();
            start->x = (float)i;
            start->y = (float)j;
            return start;
        }
    }
    return nullptr;
}

string data_exporter::get_shadow_platform_from_map_(Vector2 start, map* cells) {
    int length = 0;
    int coords_x = (int)start.x;
    while (true) {
        int cell_id = cells->at(coords_x).at((int)start.y);
        if (cell_id == -1) break;
        tile data = tileset_manager::get_tile_data(tileset::interact, cell_id);
        if (data.type != tile_type::shadow_platform) break;
        cells->at(coords_x).at((int)start.y) = -1;
        length++;
        coords_x++;
        if (coords_x == TILEMAP_SIZE_X) break;
    }
    string str = "[physics_body]\n";
    str += "pos = (" + to_string((int)start.x * SPRITESHEET_CELL_X) + "," +
           to_string((int)start.y * SPRITESHEET_CELL_Y) + ")\n";
    str += "type = fixed\n";
    str += "one_way = true\n";
    str += "collision_box = (0,0," + to_string(length * SPRITESHEET_CELL_X) +
           "," + to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
    str += "collision_layer = 00100000\n";
    return str;
}

string data_exporter::get_shadow_platform_bodies_text_(map* cells) {
    string text = "";
    Vector2* start = data_exporter::find_next_shadow_platform_start_(cells);
    while (start) {
        text += data_exporter::get_shadow_platform_from_map_(*start, cells);
        text += "\n";
        delete start;
        start = data_exporter::find_next_shadow_platform_start_(cells);
    }
    return text;
}

string data_exporter::get_shadow_platform_sprites_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::shadow_platform) continue;

            Vector2 coords = tileset_manager::get_tile_sprite_coords(
                tileset::interact, cell_id);
            text += "[sprite]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "atlas_coords = (" + to_string((int)coords.x) + "," +
                    to_string((int)coords.y) + ")\n";
            text += "tint = (0,0,255,255)\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_bg_props_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::background, cell_id);
            if (data.type != tile_type::prop) continue;
            Vector2 coords = data.spritesheet_coords;
            text += "[sprite]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "atlas_coords = (" + to_string((int)coords.x) + "," +
                    to_string((int)coords.y) + ")\n";
            text += "tint = (0,255,0,255)\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_bg_leaves_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::background, cell_id);
            if (data.type != tile_type::leaves) continue;
            text += "[falling_leaves]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_bg_drips_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::background, cell_id);
            if (data.type != tile_type::drip) continue;
            text += "[water_drip]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_bg_puffs_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::background, cell_id);
            if (data.type != tile_type::puff) continue;
            text += "[puff]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_bg_grass_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::background, cell_id);
            if (data.type != tile_type::grass) continue;
            Vector2 coords = data.spritesheet_coords;
            string frame_1_str = "(" + to_string((int)coords.x) + "," +
                                 to_string((int)coords.y) + ")";
            string frame_2_str = "(" + to_string((int)coords.x) + "," +
                                 to_string((int)coords.y + 1) + ")";
            const float grass_phase_diff = 0.3f;
            text += "[sprite]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "atlas_coords = " + frame_1_str + "\n";
            text += "tint = (255,0,0,255)\n";
            text += "animation = (" + frame_1_str + ",1.5);(" + frame_2_str +
                    ",1.0);(" + frame_1_str + ",1.0);(" + frame_2_str +
                    ",0.75)\n";
            text += "animation_starting_phase = " +
                    to_string((float)i * grass_phase_diff) + "\n";
            text += "\n";
        }
    }
    return text;
}

// string data_exporter::get_bg_waterfall_text_(map* cells) {
//     string text = "";
//     for (int i = 0; i < TILEMAP_SIZE_X; i++) {
//         for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
//             int cell_id = cells->at(i).at(j);
//             if (cell_id == -1) continue;
//             tile data =
//                 tileset_manager::get_tile_data(tileset::background, cell_id);
//             if (data.type != tile_type::waterfall) continue;
//             Vector2 coords = data.spritesheet_coords;
//             string frame_1_str = "(" + to_string((int)coords.x) + "," +
//                                  to_string((int)coords.y) + ")";
//             string frame_2_str = "(" + to_string((int)coords.x) + "," +
//                                  to_string((int)coords.y + 1) + ")";
//             text += "[sprite]\n";
//             text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
//                     to_string(j * SPRITESHEET_CELL_Y) + ")\n";
//             text += "atlas_coords = " + frame_1_str + "\n";
//             text += "tint = (0,255,0,255)\n";
//             text += "animation = (" + frame_1_str + ",0.15);(" + frame_2_str +
//                     ",0.15)\n";
//             text += "\n";
//         }
//     }
//     return text;
// }

string data_exporter::get_goal_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::goal) continue;
            int id = data_exporter::current_id_count_++;
            text += "[goal]\n";
            text += "id = " + to_string(id) + "\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "collision_box = (0,0," + to_string(SPRITESHEET_CELL_X) +
                    "," + to_string(SPRITESHEET_CELL_Y) + ")\n";
            text += "collision_mask = 00000011\n";
            text += "[sprite]\n";
            text += "parent = " + to_string(id) + "\n";
            text += "atlas_coords = (0,4)\n";
            text += "tint = (255,0,0,255)\n";
            text += "animation = ";
            text += "((0,4),0.08);";
            text += "((0,5),0.08);";
            text += "((0,6),0.08);";
            text += "((1,6),0.08);";
            text += "((2,6),0.08);";
            text += "((2,5),0.08);";
            text += "((2,4),0.08);";
            text += "((1,4),0.08)\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_bg_stars_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::background, cell_id);
            if (data.type != tile_type::star) continue;
            Vector2 coords = data.spritesheet_coords;
            string frame_1_str = "(" + to_string((int)coords.x) + "," +
                                 to_string((int)coords.y) + ")";
            string frame_2_str = "(" + to_string((int)coords.x) + "," +
                                 to_string((int)coords.y + 1) + ")";
            const int min_duration = 20;
            const int max_duration = 100;
            const float unit = 100;
            array<float, 4> durations = {
                (float)GetRandomValue(min_duration, max_duration) / unit,
                (float)GetRandomValue(min_duration, max_duration) / unit,
                (float)GetRandomValue(min_duration, max_duration) / unit,
                (float)GetRandomValue(min_duration, max_duration) / unit};
            text += "[sprite]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "atlas_coords = " + frame_1_str + "\n";
            text += "tint = (0,255,0,255)\n";
            text += "animation = (" + frame_1_str + "," +
                    to_string(durations.at(0)) + ");(" + frame_2_str + "," +
                    to_string(durations.at(1)) + ");(" + frame_1_str + "," +
                    to_string(durations.at(2)) + ");(" + frame_2_str + "," +
                    to_string(durations.at(3)) + ")\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_player_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::player) continue;
            text += "[player]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_level_text_1_text_(map* cells,
                                             string level_text,
                                             string hidden_for) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::text1) continue;
            text += "[text]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            string content = "";
            for (char i : level_text) {
                content += i == ' ' ? EXPORT_SPACE_CHAR : i;
            }
            text += "content = " + content + "\n";
            if (!hidden_for.empty())
                text += "hidden_for = " + hidden_for + "\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_level_text_2_text_(map* cells,
                                             string level_text,
                                             string hidden_for) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::text2) continue;
            text += "[text]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            string content = "";
            for (char i : level_text) {
                content += i == ' ' ? EXPORT_SPACE_CHAR : i;
            }
            text += "content = " + content + "\n";
            if (!hidden_for.empty())
                text += "hidden_for = " + hidden_for + "\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_level_text_3_text_(map* cells,
                                             string level_text,
                                             string hidden_for) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::text3) continue;
            text += "[text]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            string content = "";
            for (char i : level_text) {
                content += i == ' ' ? EXPORT_SPACE_CHAR : i;
            }
            text += "content = " + content + "\n";
            if (!hidden_for.empty())
                text += "hidden_for = " + hidden_for + "\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_water_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::water) continue;
            text += "[water]\n";
            text += "water_level = " + to_string(j * SPRITESHEET_CELL_Y) + "\n";
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_spikes_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            switch (data.type) {
            case tile_type::spike_t: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0,0," +
                        to_string(SPRITESHEET_CELL_X) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (3,4)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_b: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0," +
                        to_string(SPRITESHEET_CELL_Y / 2) + "," +
                        to_string(SPRITESHEET_CELL_X) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (3,5)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_l: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0,0," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (3,6)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_r: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 2) + ",0," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (3,7)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_v_t: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 4) + "," +
                        to_string(SPRITESHEET_CELL_Y / 4) + "," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y / 4 * 3) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (4,4)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_v_m: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 4) + ",0," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (4,5)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_v_b: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 4) + ",0," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y / 4 * 3) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (4,6)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_h_l: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 4) + "," +
                        to_string(SPRITESHEET_CELL_Y / 4) + "," +
                        to_string(SPRITESHEET_CELL_X / 4 * 3) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (5,4)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_h_m: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0," +
                        to_string(SPRITESHEET_CELL_Y / 4) + "," +
                        to_string(SPRITESHEET_CELL_X) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (5,5)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            case tile_type::spike_h_r: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0," +
                        to_string(SPRITESHEET_CELL_Y / 4) + "," +
                        to_string(SPRITESHEET_CELL_X / 4 * 3) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000011\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (5,6)\n";
                text += "tint = (255,0,0,255)\n";
                break;
            }
            default:
                continue;
            }
        }
    }
    return text;
}

string data_exporter::get_shadow_spikes_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            switch (data.type) {
            case tile_type::shadow_spike_t: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0,0," +
                        to_string(SPRITESHEET_CELL_X) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (3,4)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_b: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0," +
                        to_string(SPRITESHEET_CELL_Y / 2) + "," +
                        to_string(SPRITESHEET_CELL_X) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (3,5)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_l: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0,0," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (3,6)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_r: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 2) + ",0," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (3,7)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_v_t: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 4) + "," +
                        to_string(SPRITESHEET_CELL_Y / 4) + "," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y / 4 * 3) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (4,4)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_v_m: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 4) + ",0," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (4,5)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_v_b: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 4) + ",0," +
                        to_string(SPRITESHEET_CELL_X / 2) + "," +
                        to_string(SPRITESHEET_CELL_Y / 4 * 3) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (4,6)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_h_l: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (" +
                        to_string(SPRITESHEET_CELL_X / 4) + "," +
                        to_string(SPRITESHEET_CELL_Y / 4) + "," +
                        to_string(SPRITESHEET_CELL_X / 4 * 3) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (5,4)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_h_m: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0," +
                        to_string(SPRITESHEET_CELL_Y / 4) + "," +
                        to_string(SPRITESHEET_CELL_X) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (5,5)\n";
                text += "tint = (0,0,255,255)\n";
                break;
            }
            case tile_type::shadow_spike_h_r: {
                int id = data_exporter::current_id_count_++;
                text += "[killbox]\n";
                text += "id = " + to_string(id) + "\n";
                text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                        to_string(j * SPRITESHEET_CELL_Y) + ")\n";
                text += "collision_box = (0," +
                        to_string(SPRITESHEET_CELL_Y / 4) + "," +
                        to_string(SPRITESHEET_CELL_X / 4 * 3) + "," +
                        to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
                text += "collision_mask = 00000010\n\n";
                text += "[sprite]\n";
                text += "parent = " + to_string(id) + "\n";
                text += "atlas_coords = (5,6)\n";
                text += "tint = (255,0,255,255)\n";
                break;
            }
            default:
                continue;
            }
        }
    }
    return text;
}

string data_exporter::get_opening_obj_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::opening) continue;
            text += "[opening]\n\n";
        }
    }
    return text;
}

string data_exporter::get_credits_obj_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type != tile_type::credits) continue;
            text += "[credits]\n\n";
        }
    }
    return text;
}

string data_exporter::get_key_gate_text_(map* cells) {
    string text = "";
    Vector2 key_gate_top_pos = (Vector2){-1.0f, -1.0f};
    vector<Vector2> key_positions;
    float key_gate_height = 0.0f;
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::interact, cell_id);
            if (data.type == tile_type::key_gate_top) {
                key_gate_top_pos = (Vector2){(float)i * SPRITESHEET_CELL_X,
                                             (float)j * SPRITESHEET_CELL_Y};
                continue;
            }
            if (data.type == tile_type::key) {
                key_positions.push_back(
                    (Vector2){(float)i * SPRITESHEET_CELL_X,
                              (float)j * SPRITESHEET_CELL_Y});
                continue;
            }
            if (data.type == tile_type::key_gate) {
                key_gate_height += SPRITESHEET_CELL_Y;
                continue;
            }
        }
    }
    if (key_gate_top_pos.x >= 0.0f && !key_positions.empty() &&
        key_gate_height > 0.0f) {
        text += "[key_gate]\n";
        text += "pos = (" + to_string((int)key_gate_top_pos.x) + "," +
                to_string((int)key_gate_top_pos.y) + ")\n";
        text += "height = " + to_string((int)key_gate_height) + "\n";
        text += "key_positions = ";
        for (Vector2 pos : key_positions) {
            text += "(" + to_string((int)pos.x) + "," + to_string((int)pos.y) +
                    ");";
        }
        text.erase(text.length() - 1);
        text += "\n\n";
    }
    return text;
}

string data_exporter::get_level_shadow_totals_(string level_shadows) {
    string text = "[shadows_limit]\n";
    text += level_shadows.empty() ? "0" : level_shadows;
    text += "\n\n";
    return text;
}

string data_exporter::get_screen_colors_shader_(string screen_colors_shader) {
    string text = "[screen_colors_shader]\n";
    text += screen_colors_shader.empty() ? "0" : screen_colors_shader;
    text += "\n\n";
    return text;
}

string data_exporter::get_export_text(unordered_map<tileset, map> cells,
                                      string level_text_1,
                                      string text_1_hidden_for,
                                      string level_text_2,
                                      string text_2_hidden_for,
                                      string level_text_3,
                                      string text_3_hidden_for,
                                      string level_shadows,
                                      string screen_colors_shader) {
    data_exporter::current_id_count_ = 0;
    string data = "";
    data += data_exporter::get_out_of_bounds_walls_();
    data += data_exporter::get_bg_props_text_(&cells.at(tileset::background));
    data+= data_exporter::get_bg_grass_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_stars_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_leaves_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_drips_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_puffs_text_(&cells.at(tileset::background));
    data += data_exporter::get_blocks_sprites_text_(&cells.at(tileset::blocks));
    data += data_exporter::get_physics_bodies_text_(&cells.at(tileset::blocks));
    data += data_exporter::get_spikes_text_(&cells.at(tileset::interact));
    data += data_exporter::get_shadow_spikes_text_(&cells.at(tileset::interact));
    data += data_exporter::get_water_text_(&cells.at(tileset::interact));
    data += data_exporter::get_key_gate_text_(&cells.at(tileset::interact));
    data += data_exporter::get_player_text_(&cells.at(tileset::interact));
    data += data_exporter::get_goal_text_(&cells.at(tileset::interact));
    data += data_exporter::get_level_text_1_text_(
        &cells.at(tileset::interact), level_text_1, text_1_hidden_for);
    data += data_exporter::get_level_text_2_text_(
        &cells.at(tileset::interact), level_text_2, text_2_hidden_for);
    data += data_exporter::get_level_text_3_text_(
        &cells.at(tileset::interact), level_text_3, text_3_hidden_for);
    data += data_exporter::get_opening_obj_text_(&cells.at(tileset::interact));
    data += data_exporter::get_credits_obj_text_(&cells.at(tileset::interact));
    data +=
        data_exporter::get_platform_sprites_text_(&cells.at(tileset::interact));
    data +=
        data_exporter::get_platform_bodies_text_(&cells.at(tileset::interact));
    data +=
        data_exporter::get_shadow_platform_sprites_text_(&cells.at(tileset::interact));
    data +=
        data_exporter::get_shadow_platform_bodies_text_(&cells.at(tileset::interact));

    data += data_exporter::get_level_shadow_totals_(level_shadows);
    data += data_exporter::get_screen_colors_shader_(screen_colors_shader);

    return data;
}
