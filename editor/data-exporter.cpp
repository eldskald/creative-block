#include "data-exporter.h"
#include "defs.h"
#include "tilemap.h"
#include "tileset-manager.h"
#include <array>
#include <raylib.h>
#include <string>
#include <unordered_map>

int data_exporter::current_id_count_ = 0;

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

string data_exporter::get_bg_waterfall_text_(map* cells) {
    string text = "";
    for (int i = 0; i < TILEMAP_SIZE_X; i++) {
        for (int j = 0; j < TILEMAP_SIZE_Y; j++) {
            int cell_id = cells->at(i).at(j);
            if (cell_id == -1) continue;
            tile data =
                tileset_manager::get_tile_data(tileset::background, cell_id);
            if (data.type != tile_type::waterfall) continue;
            Vector2 coords = data.spritesheet_coords;
            string frame_1_str = "(" + to_string((int)coords.x) + "," +
                                 to_string((int)coords.y) + ")";
            string frame_2_str = "(" + to_string((int)coords.x) + "," +
                                 to_string((int)coords.y + 1) + ")";
            text += "[sprite]\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "atlas_coords = " + frame_1_str + "\n";
            text += "tint = (0,255,0,255)\n";
            text += "animation = (" + frame_1_str + ",0.15);(" + frame_2_str +
                    ",0.15)\n";
            text += "\n";
        }
    }
    return text;
}

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

string data_exporter::get_level_text_1_text_(map* cells, string level_text) {
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
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_level_text_2_text_(map* cells, string level_text) {
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
            text += "\n";
        }
    }
    return text;
}

string data_exporter::get_level_text_3_text_(map* cells, string level_text) {
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
            int id = data_exporter::current_id_count_++;
            text += "[killbox]\n";
            text += "id = " + to_string(id) + "\n";
            text += "pos = (" + to_string(i * SPRITESHEET_CELL_X) + "," +
                    to_string(j * SPRITESHEET_CELL_Y) + ")\n";
            text += "collision_box = (0,0," + to_string(SPRITESHEET_CELL_X) +
                    "," + to_string(SPRITESHEET_CELL_Y / 2) + ")\n";
            text += "collision_mask = 00000011\n";
            text += "[sprite]\n";
            text += "parent = " + to_string(id) + "\n";
            text += "tint = (255,0,0,255)\n";
            text += "atlas_coords = (7,4)\n";
            const float frame_time = 0.1f;
            const int frame_total = 32;
            const float x_frame_diff = 8.0f;
            text += "animation = ";
            text += "((7,4)," + to_string(frame_time) + ");";
            text += "((7,5)," + to_string(frame_time) + ");";
            text += "((7,6)," + to_string(frame_time) + ");";
            text += "((7,7)," + to_string(frame_time) + ");";
            text += "((8,4)," + to_string(frame_time) + ");";
            text += "((8,5)," + to_string(frame_time) + ");";
            text += "((8,6)," + to_string(frame_time) + ");";
            text += "((8,7)," + to_string(frame_time) + ");";
            text += "((9,4)," + to_string(frame_time) + ");";
            text += "((9,5)," + to_string(frame_time) + ");";
            text += "((9,6)," + to_string(frame_time) + ");";
            text += "((9,7)," + to_string(frame_time) + ");";
            text += "((10,4)," + to_string(frame_time) + ");";
            text += "((10,5)," + to_string(frame_time) + ");";
            text += "((10,6)," + to_string(frame_time) + ");";
            text += "((10,7)," + to_string(frame_time) + ");";
            text += "((11,4)," + to_string(frame_time) + ");";
            text += "((11,5)," + to_string(frame_time) + ");";
            text += "((11,6)," + to_string(frame_time) + ");";
            text += "((11,7)," + to_string(frame_time) + ");";
            text += "((12,4)," + to_string(frame_time) + ");";
            text += "((12,5)," + to_string(frame_time) + ");";
            text += "((12,6)," + to_string(frame_time) + ");";
            text += "((12,7)," + to_string(frame_time) + ");";
            text += "((13,4)," + to_string(frame_time) + ");";
            text += "((13,5)," + to_string(frame_time) + ");";
            text += "((13,6)," + to_string(frame_time) + ");";
            text += "((13,7)," + to_string(frame_time) + ");";
            text += "((14,4)," + to_string(frame_time) + ");";
            text += "((14,5)," + to_string(frame_time) + ");";
            text += "((14,6)," + to_string(frame_time) + ");";
            text += "((14,7)," + to_string(frame_time) + ")\n";
            text += "animation_starting_phase = " +
                    to_string((float)(i % frame_total) * frame_time *
                              x_frame_diff) +
                    "\n";
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

string data_exporter::get_level_shadow_totals_(string level_shadows) {
    string text = "[shadows_limit]\n";
    text += level_shadows.empty() ? "0" : level_shadows;
    text += "\n";
    return text;
}

string data_exporter::get_export_text(unordered_map<tileset, map> cells,
                                      string level_text_1,
                                      string level_text_2,
                                      string level_text_3,
                                      string level_shadows) {
    data_exporter::current_id_count_ = 0;
    string data = "";
    data += data_exporter::get_bg_props_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_grass_text_(&cells.at(tileset::background));
    data +=
        data_exporter::get_bg_waterfall_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_stars_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_leaves_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_drips_text_(&cells.at(tileset::background));
    data += data_exporter::get_bg_puffs_text_(&cells.at(tileset::background));
    data += data_exporter::get_blocks_sprites_text_(&cells.at(tileset::blocks));
    data += data_exporter::get_physics_bodies_text_(&cells.at(tileset::blocks));
    data += data_exporter::get_spikes_text_(&cells.at(tileset::interact));
    data += data_exporter::get_water_text_(&cells.at(tileset::interact));
    data += data_exporter::get_player_text_(&cells.at(tileset::interact));
    data += data_exporter::get_goal_text_(&cells.at(tileset::interact));
    data += data_exporter::get_level_text_1_text_(&cells.at(tileset::interact),
                                                  level_text_1);
    data += data_exporter::get_level_text_2_text_(&cells.at(tileset::interact),
                                                  level_text_2);
    data += data_exporter::get_level_text_3_text_(&cells.at(tileset::interact),
                                                  level_text_3);
    data += data_exporter::get_opening_obj_text_(&cells.at(tileset::interact));
    data += data_exporter::get_credits_obj_text_(&cells.at(tileset::interact));
    data +=
        data_exporter::get_platform_sprites_text_(&cells.at(tileset::interact));
    data +=
        data_exporter::get_platform_bodies_text_(&cells.at(tileset::interact));
    data += data_exporter::get_level_shadow_totals_(level_shadows);

    return data;
}
