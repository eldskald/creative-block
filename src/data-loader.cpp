#include "data-loader.h"
#include "game-element.h"
#include "physics-body.h"
#include "sprite.h"
#include <cctype>
#include <exception>
#include <iostream>
#include <list>
#include <raylib.h>
#include <stdexcept>
#include <string>
#include <vector>

struct element_block {
    string type;
    vector<string> properties;
};

string remove_whitespace(string text) {
    for (int i = 0; i < text.size(); ++i) {
        if (isspace(text[i])) {
            text.erase(i, 1);
            i = 0;
        }
    }
    return text;
}

vector<string> split_into_lines_with_no_whitespace(string text) {
    vector<string> lines;
    while (!text.empty()) {
        string line = text.substr(0, text.find('\n'));
        if (!line.empty()) lines.push_back(remove_whitespace(line));
        text.erase(0, text.find('\n') + 1);
    }
    return lines;
}

bool check_if_line_starts_a_block(string line) {
    return line[0] == '[' && line[line.size() - 1] == ']';
}

element_block get_top_element_block(vector<string>* data) {
    if (!check_if_line_starts_a_block((*data)[0]))
        throw invalid_argument("first line not a block starter");

    element_block block;
    block.type = (*data)[0].substr(1, (*data)[0].size() - 2);
    (*data).erase((*data).begin());
    while (!(*data).empty()) {
        if (check_if_line_starts_a_block((*data)[0])) break;
        block.properties.push_back((*data)[0]);
        (*data).erase((*data).begin());
    }
    return block;
}

Vector2 string_to_vector(string str) {
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

Rectangle string_to_rectangle(string str) {
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

Color string_to_color(string str) {
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

bool string_to_bool(string str) {
    if (str == "true") return true;
    if (str == "false") return false;
    throw invalid_argument("value not a bool");
}

physics_body::body_type string_to_body_type(string str) {
    if (str == "kinematic") return physics_body::kinematic;
    if (str == "fixed") return physics_body::fixed;
    if (str == "area") return physics_body::area;
    throw invalid_argument("value not a body type");
}

void parse_game_element_property_line(game_element* element, string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "pos") {
        element->pos = string_to_vector(prop_value);
    } else
        throw invalid_argument("invalid property name");
}

void parse_physics_body_property_line(physics_body* body, string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "pos") {
        body->pos = string_to_vector(prop_value);
    } else if (prop_name == "type") {
        body->type = string_to_body_type(prop_value);
    } else if (prop_name == "one_way") {
        body->one_way = string_to_bool(prop_value);
    } else if (prop_name == "collision_box") {
        body->collision_box = string_to_rectangle(prop_value);
    } else if (prop_name == "collision_layer") {
        body->collision_layer = bitset<COLLISION_LAYERS>(prop_value);
    } else if (prop_name == "collision_mask") {
        body->collision_mask = bitset<COLLISION_LAYERS>(prop_value);
    } else
        throw invalid_argument("invalid property name");
}

void parse_sprite_property_line(sprite* sprite, string line) {
    if (line.find('=') == line.npos)
        throw invalid_argument("property line has no '=' sign");

    string prop_name = line.substr(0, line.find('='));
    string prop_value =
        line.substr(line.find('=') + 1, line.size() - line.find('=') - 1);
    if (prop_name == "pos") {
        sprite->pos = string_to_vector(prop_value);
    } else if (prop_name == "atlas_coords") {
        sprite->atlas_coords = string_to_vector(prop_value);
    } else if (prop_name == "tint") {
        sprite->tint = string_to_color(prop_value);
    } else
        throw invalid_argument("invalid property name");
}

game_element* get_element_from_block(element_block block) {
    if (block.type == "game_element") {
        auto* obj = new game_element();
        for (auto& line : block.properties) {
            parse_game_element_property_line(obj, line);
        }
        return obj;
    }
    if (block.type == "physics_body") {
        auto* body = new physics_body();
        for (auto& line : block.properties) {
            parse_physics_body_property_line(body, line);
        }
        return body;
    }
    if (block.type == "sprite") {
        auto* img = new sprite();
        for (auto& line : block.properties) {
            parse_sprite_property_line(img, line);
        }
        return img;
    }
    throw invalid_argument("invalid block type");
}

list<game_element*> data_loader::load(const char* file) {
    vector<string> lines =
        split_into_lines_with_no_whitespace(LoadFileText(file));

    list<element_block> blocks;
    list<game_element*> elements;
    try {
        while (!lines.empty()) {
            element_block block = get_top_element_block(&lines);
            blocks.push_back(block);
        }
        for (auto& block : blocks) {
            elements.push_back(get_element_from_block(block));
        }
    } catch (invalid_argument& e) {
        cout << e.what() << endl;
        CloseWindow();
    }

    return elements;
}
