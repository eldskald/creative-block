#pragma once
#include "core/key-gate.h"
#include "core/player.h"
#include "core/water.h"
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "engine/text.h"
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

class data_loader {
public:
    static list<game_element*> load(const char* file);

private:
    struct element_block {
        string type;
        vector<string> properties;
    };

    static unordered_map<string, game_element*> id_map_;

    static string remove_whitespace_(string text);
    static vector<string> split_into_lines_with_no_whitespace_(string text);

    static bool check_if_line_starts_a_block_(string line);
    static element_block get_top_element_block_(vector<string>* data);
    static game_element* get_element_from_block_(element_block block);

    static vector<string> string_to_array_(string str);
    static Vector2 string_to_vector_(string str);
    static Rectangle string_to_rectangle_(string str);
    static Color string_to_color_(string str);
    static bool string_to_bool_(string str);
    static physics_body::body_type string_to_body_type_(string str);
    static keyframe string_to_keyframe_(string str);
    static animation string_to_animation_(string str);
    static string string_to_string_(string str);

    static void add_parent_(game_element* element, string id);

    static void parse_game_element_property_line_(game_element* element,
                                                  string line);
    static void parse_physics_body_property_line_(physics_body* body,
                                                  string line);
    static void parse_sprite_property_line_(sprite* sprite, string line);
    static void parse_text_property_line_(text* text, string line);
    static void parse_player_property_line_(player* player, string line);
    static void parse_water_property_line_(water* water, string line);
    static void parse_key_gate_property_line_(key_gate* gate, string line);
    static void parse_level_shadows_limit_line_(string line);
    static void parse_screen_colors_shader_line_(string line);
};
