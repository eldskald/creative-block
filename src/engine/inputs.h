#pragma once
#include <raylib.h>
#include <unordered_map>
#include <vector>

using namespace std;

class game;

class inputs {
    friend class game;

public:
    enum action CONTROLS_ACTIONS;

    static void initialize();
    static Vector2 get_dir_input();
    static bool did_dir_input_change();
    static bool is_action_pressed(action input);
    static bool is_action_released(action input);
    static bool is_action_down(action input);
    static bool is_action_up(action input);

private:
    static unordered_map<action, vector<int>> action_buttons_;
    static unordered_map<action, vector<int>> action_keys_;
    static unordered_map<action, bool> action_down_;
    static unordered_map<action, bool> action_pressed_;
    static unordered_map<action, bool> action_released_;
    static Vector2 dir_input_;
    static bool dir_input_changed_;

    static void tick_();
    static bool is_action_down_(action input);
};
