#pragma once
#include "game-element.h"
#include "physics-body.h"
#include "sprite.h"
#include <list>
#include <string>
#include <vector>

class data_loader {
public:
    static list<game_element*> load(const char* file);

private:
    struct element_block {
        string type;
        vector<string> properties;
    };

    static string remove_whitespace(string text);
    static vector<string> split_into_lines_with_no_whitespace(string text);

    static bool check_if_line_starts_a_block(string line);
    static element_block get_top_element_block(vector<string>* data);
    static game_element* get_element_from_block(element_block block);

    static Vector2 string_to_vector(string str);
    static Rectangle string_to_rectangle(string str);
    static Color string_to_color(string str);
    static bool string_to_bool(string str);
    static physics_body::body_type string_to_body_type(string str);
    static Shader* string_to_shader(string str);

    static void parse_game_element_property_line(game_element* element,
                                                 string line);
    static void parse_physics_body_property_line(physics_body* body,
                                                 string line);
    static void parse_sprite_property_line(sprite* sprite, string line);
};
