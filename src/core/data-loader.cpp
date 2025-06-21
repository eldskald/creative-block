#include "core/data-loader.h"
#include "core/credits.h"
#include "core/falling-leaves.h"
#include "core/goal.h"
#include "core/key-gate.h"
#include "core/killbox.h"
#include "core/opening.h"
#include "core/player.h"
#include "core/puff.h"
#include "core/scene-manager.h"
#include "core/water-drip.h"
#include "core/water.h"
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "engine/text.h"
#include <cctype>
#include <exception>
#include <list>
#include <raylib.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

unordered_map<string, game_element*> data_loader::id_map_ = {};

string data_loader::remove_whitespace_(string text) {
    for (int i = 0; i < (int)text.size(); ++i) {
        if (isspace(text[i])) {
            text.erase(i, 1);
            i = 0;
        }
    }
    return text;
}

vector<string> data_loader::split_into_lines_with_no_whitespace_(string text) {
    vector<string> lines;
    while (!text.empty()) {
        string line = text.substr(0, text.find('\n'));
        if (!line.empty())
            lines.push_back(data_loader::remove_whitespace_(line));
        text.erase(0, text.find('\n') + 1);
    }
    return lines;
}

bool data_loader::check_if_line_starts_a_block_(string line) {
    return line[0] == '[' && line[line.size() - 1] == ']';
}

data_loader::element_block
data_loader::get_top_element_block_(vector<string>* data) {
    if (!data_loader::check_if_line_starts_a_block_((*data)[0]))
        throw invalid_argument("first line not a block starter");

    element_block block;
    block.type = (*data)[0].substr(1, (*data)[0].size() - 2);
    (*data).erase((*data).begin());
    while (!(*data).empty()) {
        if (data_loader::check_if_line_starts_a_block_((*data)[0])) break;
        block.properties.push_back((*data)[0]);
        (*data).erase((*data).begin());
    }
    return block;
}

vector<string> data_loader::string_to_array_(string str) {
    vector<string> arr;
    while (true) {
        size_t next_semicolon = str.find(';');
        string next_element = str.substr(0, next_semicolon);
        arr.push_back(next_element);
        if (next_semicolon == str.npos) break;
        str.erase(0, next_element.size() + 1);
    }
    return arr;
}

Vector2 data_loader::string_to_vector_(string str) {
    if (str[0] != '(' || str[str.size() - 1] != ')')
        throw invalid_argument("value not a vector");
    if (str.find(',') == str.npos) throw invalid_argument("value not a vector");

    Vector2 vec = (Vector2){0};
    string x_value = str.substr(1, str.find(',') - 1);
    string y_value =
        str.substr(str.find(',') + 1, str.size() - str.find(',') - 2);
    try {
        vec.x = stof(x_value);
        vec.y = stof(y_value);
    } catch (exception& e) {
        throw invalid_argument("value not a vector");
    }

    return vec;
}

Rectangle data_loader::string_to_rectangle_(string str) {
    if (str[0] != '(' || str[str.size() - 1] != ')')
        throw invalid_argument("value not a rectangle");

    string rect_str = str.substr(1, str.size() - 2);
    Rectangle rect = (Rectangle){0};
    for (int i = 0; i < 4; i++) {
        if (i < 3 && rect_str.find(',') == rect_str.npos)
            throw invalid_argument("value not a rectangle");

        try {
            switch (i) {
            case 0: {
                rect.x = stof(rect_str.substr(0, rect_str.find(',')));
                rect_str.erase(0, rect_str.find(',') + 1);
                break;
            }
            case 1: {
                rect.y = stof(rect_str.substr(0, rect_str.find(',')));
                rect_str.erase(0, rect_str.find(',') + 1);
                break;
            }
            case 2: {
                rect.width = stof(rect_str.substr(0, rect_str.find(',')));
                rect_str.erase(0, rect_str.find(',') + 1);
                break;
            }
            case 3: {
                rect.height = stof(rect_str);
                break;
            }
            }
        } catch (exception& e) {
            throw invalid_argument("value not a rectangle");
        }
    }

    return rect;
}

Color data_loader::string_to_color_(string str) {
    if (str[0] != '(' || str[str.size() - 1] != ')')
        throw invalid_argument("value not a color");

    string col_str = str.substr(1, str.size() - 2);
    Color col = (Color){0};
    for (int i = 0; i < 4; i++) {
        if (i < 3 && col_str.find(',') == col_str.npos)
            throw invalid_argument("value not a color");

        try {
            switch (i) {
            case 0: {
                col.r = stoi(col_str.substr(0, col_str.find(',')));
                col_str.erase(0, col_str.find(',') + 1);
                break;
            }
            case 1: {
                col.g = stoi(col_str.substr(0, col_str.find(',')));
                col_str.erase(0, col_str.find(',') + 1);
                break;
            }
            case 2: {
                col.b = stoi(col_str.substr(0, col_str.find(',')));
                col_str.erase(0, col_str.find(',') + 1);
                break;
            }
            case 3: {
                col.a = stoi(col_str);
                break;
            }
            }
        } catch (exception& e) {
            throw invalid_argument("value not a color");
        }
    }

    return col;
}

bool data_loader::string_to_bool_(string str) {
    if (str == "true") return true;
    if (str == "false") return false;
    throw invalid_argument("value not a bool");
}

physics_body::body_type data_loader::string_to_body_type_(string str) {
    if (str == "kinematic") return physics_body::kinematic;
    if (str == "fixed") return physics_body::fixed;
    if (str == "area") return physics_body::area;
    throw invalid_argument("value not a body type");
}

keyframe data_loader::string_to_keyframe_(string str) {
    if (str[0] != '(' || str[str.size() - 1] != ')')
        throw invalid_argument("value not a keyframe");
    string data = str.substr(1, str.size() - 2);
    if (data[0] != '(' || data.find(')') == data.npos)
        throw invalid_argument("value not a keyframe");
    string atlas_coords_element = data.substr(0, data.find(')') + 1);
    data.erase(0, atlas_coords_element.size());
    if (data[0] != ',') throw invalid_argument("value not a keyframe");
    string duration_element = data.substr(1, data.size() - 1);

    keyframe frame = (keyframe){(Vector2){0}, 0.0f};
    frame.atlas_coords = data_loader::string_to_vector_(atlas_coords_element);
    frame.duration = stof(duration_element);
    return frame;
}

animation data_loader::string_to_animation_(string str) {
    vector<string> frames_string = data_loader::string_to_array_(str);
    animation anim;
    for (string frame_string : frames_string) {
        anim.push_back(data_loader::string_to_keyframe_(frame_string));
    }
    return anim;
}

string data_loader::string_to_string_(string str) {
    string r = "";
    for (char i : str) {
        r += i == '@' ? ' ' : i;
    }
    return r;
}

void data_loader::add_parent_(game_element* element, string id) {
    if (data_loader::id_map_.count(id) == 0)
        throw invalid_argument("invalid parent id");
    data_loader::id_map_[id]->add_child(element);
}

void data_loader::parse_game_element_property_line_(game_element* element,
                                                    string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "id") {
        data_loader::id_map_[prop_value] = element;
    } else if (prop_name == "parent") {
        data_loader::add_parent_(element, prop_value);
    } else if (prop_name == "pos") {
        element->pos = data_loader::string_to_vector_(prop_value);
    } else
        throw invalid_argument("invalid property name");
}

void data_loader::parse_physics_body_property_line_(physics_body* body,
                                                    string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "id") {
        data_loader::id_map_[prop_value] = body;
    } else if (prop_name == "parent") {
        data_loader::add_parent_(body, prop_value);
    } else if (prop_name == "pos") {
        body->pos = data_loader::string_to_vector_(prop_value);
    } else if (prop_name == "type") {
        body->type = data_loader::string_to_body_type_(prop_value);
    } else if (prop_name == "one_way") {
        body->one_way = data_loader::string_to_bool_(prop_value);
    } else if (prop_name == "collision_box") {
        body->collision_box = data_loader::string_to_rectangle_(prop_value);
    } else if (prop_name == "collision_layer") {
        body->collision_layer = bitset<COLLISION_LAYERS>(prop_value);
    } else if (prop_name == "collision_mask") {
        body->collision_mask = bitset<COLLISION_LAYERS>(prop_value);
    } else
        throw invalid_argument("invalid property name");
}

void data_loader::parse_sprite_property_line_(sprite* sprite, string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "id") {
        data_loader::id_map_[prop_value] = sprite;
    } else if (prop_name == "parent") {
        data_loader::add_parent_(sprite, prop_value);
    } else if (prop_name == "pos") {
        sprite->pos = data_loader::string_to_vector_(prop_value);
    } else if (prop_name == "atlas_coords") {
        sprite->atlas_coords = data_loader::string_to_vector_(prop_value);
    } else if (prop_name == "tint") {
        sprite->tint = data_loader::string_to_color_(prop_value);
    } else if (prop_name == "z_index") {
        sprite->z_index = stoi(prop_value);
    } else if (prop_name == "animation") {
        sprite->anim = data_loader::string_to_animation_(prop_value);
    } else if (prop_name == "animation_starting_phase") {
        sprite->animation_starting_phase = stof(prop_value);
    } else
        throw invalid_argument("invalid property name");
}

void data_loader::parse_text_property_line_(text* text, string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "id") {
        data_loader::id_map_[prop_value] = text;
    } else if (prop_name == "parent") {
        data_loader::add_parent_(text, prop_value);
    } else if (prop_name == "pos") {
        text->pos = data_loader::string_to_vector_(prop_value);
    } else if (prop_name == "content") {
        text->content = data_loader::string_to_string_(prop_value);
    } else if (prop_name == "hidden_for") {
        text->hidden_for = stof(prop_value);
    }
}

void data_loader::parse_player_property_line_(player* player, string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "id") {
        data_loader::id_map_[prop_value] = player;
    } else if (prop_name == "parent") {
        data_loader::add_parent_(player, prop_value);
    } else if (prop_name == "pos") {
        player->pos = data_loader::string_to_vector_(prop_value);
    }
}

void data_loader::parse_water_property_line_(water* water, string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "water_level") {
        water->water_level = stof(prop_value);
    }
}

void data_loader::parse_key_gate_property_line_(key_gate* gate, string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "pos") {
        gate->pos = data_loader::string_to_vector_(prop_value);
    } else if (prop_name == "height") {
        gate->height = stof(prop_value);
    } else if (prop_name == "key_positions") {
        for (string str_value : data_loader::string_to_array_(prop_value)) {
            gate->key_positions.push_back(
                data_loader::string_to_vector_(str_value));
        }
    }
}

void data_loader::parse_level_shadows_limit_line_(string line) {
    scene_manager::set_shadows_limit(stoi(line));
}

game_element* data_loader::get_element_from_block_(element_block block) {
    if (block.type == "game_element") {
        auto* obj = new game_element();
        for (auto& line : block.properties) {
            data_loader::parse_game_element_property_line_(obj, line);
        }
        return obj;
    }
    if (block.type == "physics_body") {
        auto* body = new physics_body();
        for (auto& line : block.properties) {
            data_loader::parse_physics_body_property_line_(body, line);
        }
        return body;
    }
    if (block.type == "sprite") {
        auto* img = new sprite();
        for (auto& line : block.properties) {
            data_loader::parse_sprite_property_line_(img, line);
        }
        return img;
    }
    if (block.type == "text") {
        auto* txt = new text();
        for (auto& line : block.properties) {
            data_loader::parse_text_property_line_(txt, line);
        }
        return txt;
    }
    if (block.type == "player") {
        auto* player_element = new player();
        for (auto& line : block.properties) {
            data_loader::parse_player_property_line_(player_element, line);
        }
        return player_element;
    }
    if (block.type == "goal") {
        auto* goal_element = new goal();
        for (auto& line : block.properties) {
            data_loader::parse_physics_body_property_line_(goal_element, line);
        }
        return goal_element;
    }
    if (block.type == "water") {
        auto* water_element = new water();
        for (auto& line : block.properties) {
            data_loader::parse_water_property_line_(water_element, line);
        }
        return water_element;
    }
    if (block.type == "falling_leaves") {
        auto* emitter = new falling_leaves();
        for (auto& line : block.properties) {
            data_loader::parse_game_element_property_line_(emitter, line);
        }
        return emitter;
    }
    if (block.type == "water_drip") {
        auto* emitter = new water_drip();
        for (auto& line : block.properties) {
            data_loader::parse_game_element_property_line_(emitter, line);
        }
        return emitter;
    }
    if (block.type == "puff") {
        auto* emitter = new puff();
        for (auto& line : block.properties) {
            data_loader::parse_game_element_property_line_(emitter, line);
        }
        return emitter;
    }
    if (block.type == "killbox") {
        auto* killer = new killbox();
        for (auto& line : block.properties) {
            data_loader::parse_physics_body_property_line_(killer, line);
        }
        return killer;
    }
    if (block.type == "key_gate") {
        auto* gate = new key_gate();
        for (auto& line : block.properties) {
            data_loader::parse_key_gate_property_line_(gate, line);
        }
        return gate;
    }
    if (block.type == "shadows_limit") {
        for (auto& line : block.properties) {
            data_loader::parse_level_shadows_limit_line_(line);
        }
        return nullptr;
    }
    if (block.type == "opening") {
        return new opening();
    }
    if (block.type == "credits") {
        return new credits();
    }
    throw invalid_argument("invalid block type");
}

list<game_element*> data_loader::load(const char* file) {
    char* text = LoadFileText(file);

    vector<string> lines =
        data_loader::split_into_lines_with_no_whitespace_(text);
    list<element_block> blocks;
    list<game_element*> elements;
    try {
        while (!lines.empty()) {
            element_block block = data_loader::get_top_element_block_(&lines);
            blocks.push_back(block);
        }
        for (auto block : blocks) {
            game_element* element = data_loader::get_element_from_block_(block);
            if (element) elements.push_back(element);
        }
    } catch (invalid_argument& e) {
        TraceLog(LOG_FATAL, e.what());
        CloseWindow();
    }
    TraceLog(LOG_INFO, "Scene loaded successfully");

    UnloadFileText(text);
    return elements;
}
