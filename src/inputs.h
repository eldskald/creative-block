#pragma once
#include <unordered_map>
#include <vector>

using namespace std;

class inputs {
public:
    enum action { move_left, move_right, jump, spawn_shadow, reset };

    static bool is_action_pressed(action input);
    static bool is_action_down(action input);
    static bool is_action_up(action input);

private:
    static unordered_map<action, vector<int>> action_buttons_;
    static unordered_map<action, vector<int>> action_keys_;
};
