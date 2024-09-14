#include "inputs.h"
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <vector>

unordered_map<inputs::action, vector<int>> inputs::action_keys_ = CONTROLS_KEYBOARD;
unordered_map<inputs::action, vector<int>> inputs::action_buttons_ = CONTROLS_GAMEPAD;
unordered_map<inputs::action, bool> inputs::action_down_ = {};
unordered_map<inputs::action, bool> inputs::action_pressed_ = {};
unordered_map<inputs::action, bool> inputs::action_released_ = {};

Vector2 inputs::dir_input_ = (Vector2){0};

bool inputs::dir_input_changed_ = false;

void inputs::initialize() {
    for (auto action : CONTROLS_ACTIONS) {
        inputs::action_down_.insert({action, false});
        inputs::action_pressed_.insert({action, false});
        inputs::action_released_.insert({action, false});
    }
}

void inputs::tick_() {

    // Updating dir input
    Vector2 new_dir_input =
        (Vector2){(float)(inputs::is_action_down_(action::right)) -
                      (float)(inputs::is_action_down_(action::left)),
                  (float)inputs::is_action_down_(action::down) -
                      (float)(inputs::is_action_down_(action::up))};
    inputs::dir_input_changed_ =
        !Vector2Equals(new_dir_input, inputs::dir_input_);
    inputs::dir_input_ = new_dir_input;

    // Update action states
    for (auto action : CONTROLS_ACTIONS) {
        action_pressed_[action] = false;
        action_released_[action] = false;
        if (!action_down_[action] && inputs::is_action_down_(action)) {
            action_pressed_[action] = true;
        } else if (action_down_[action] && !inputs::is_action_down_(action)) {
            action_released_[action] = true;
        }
        action_down_[action] = inputs::is_action_down_(action);
    }
}

bool inputs::is_action_down_(action act) {
    for (auto button : inputs::action_buttons_[act]) {
        if (IsGamepadButtonDown(0, button)) return true;
    }
    for (auto key : inputs::action_keys_[act]) {
        if (IsKeyDown(key)) return true;
    }
    return false;
}

bool inputs::is_action_pressed(action act) {
    return inputs::action_pressed_[act];
}

bool inputs::is_action_released(action act) {
    return inputs::action_released_[act];
}

bool inputs::is_action_down(action act) {
    return inputs::action_down_[act];
}

bool inputs::is_action_up(action act) {
    return !inputs::action_down_[act];
}

Vector2 inputs::get_dir_input() {
    return (Vector2){inputs::dir_input_.x, inputs::dir_input_.y};
}

bool inputs::did_dir_input_change() {
    return inputs::dir_input_changed_;
}
