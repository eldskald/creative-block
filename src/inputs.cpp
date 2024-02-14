#include "inputs.h"
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <vector>

unordered_map<inputs::action, vector<int>> inputs::action_buttons_ = {
    {action::left, vector<int>{GAMEPAD_BUTTON_LEFT_FACE_LEFT}},
    {action::right, vector<int>{GAMEPAD_BUTTON_LEFT_FACE_RIGHT}},
    {action::up, vector<int>{GAMEPAD_BUTTON_LEFT_FACE_UP}},
    {action::down, vector<int>{GAMEPAD_BUTTON_LEFT_FACE_DOWN}},
    {action::jump,
     vector<int>{GAMEPAD_BUTTON_RIGHT_FACE_DOWN,
                 GAMEPAD_BUTTON_RIGHT_FACE_LEFT,
                 GAMEPAD_BUTTON_RIGHT_FACE_UP,
                 GAMEPAD_BUTTON_RIGHT_FACE_RIGHT}},
    {action::spawn_shadow,
     vector<int>{GAMEPAD_BUTTON_LEFT_TRIGGER_1, GAMEPAD_BUTTON_LEFT_TRIGGER_2}},
    {action::reset, vector<int>{GAMEPAD_BUTTON_MIDDLE_RIGHT}},
};

unordered_map<inputs::action, vector<int>> inputs::action_keys_ = {
    {action::left, vector<int>{KEY_A, KEY_LEFT}},
    {action::right, vector<int>{KEY_D, KEY_RIGHT}},
    {action::up, vector<int>{KEY_W, KEY_UP}},
    {action::down, vector<int>{KEY_S, KEY_DOWN}},
    {action::jump, vector<int>{KEY_J, KEY_Z}},
    {action::spawn_shadow, vector<int>{KEY_K}},
    {action::reset, vector<int>{KEY_BACKSPACE}},
};

unordered_map<inputs::action, bool> inputs::action_down_ = {
    {action::left, false},
    {action::right, false},
    {action::up, false},
    {action::down, false},
    {action::jump, false},
    {action::spawn_shadow, false},
    {action::reset, false},
};

unordered_map<inputs::action, bool> inputs::action_pressed_ = {
    {action::left, false},
    {action::right, false},
    {action::up, false},
    {action::down, false},
    {action::jump, false},
    {action::spawn_shadow, false},
    {action::reset, false},
};

unordered_map<inputs::action, bool> inputs::action_released_ = {
    {action::left, false},
    {action::right, false},
    {action::up, false},
    {action::down, false},
    {action::jump, false},
    {action::spawn_shadow, false},
    {action::reset, false},
};

Vector2 inputs::dir_input_ = (Vector2){0};

bool inputs::dir_input_changed_ = false;

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
    for (auto [action, _value] : action_buttons_) {
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
