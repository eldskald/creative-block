#include "inputs.h"
#include <raylib.h>
#include <unordered_map>
#include <vector>

unordered_map<inputs::action, vector<int>> inputs::action_buttons_ = {
    {action::move_left, vector<int>{GAMEPAD_BUTTON_LEFT_FACE_LEFT}},
    {action::move_right, vector<int>{GAMEPAD_BUTTON_LEFT_FACE_RIGHT}},
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
    {action::move_left, vector<int>{KEY_A, KEY_LEFT}},
    {action::move_right, vector<int>{KEY_D, KEY_RIGHT}},
    {action::jump, vector<int>{KEY_J, KEY_Z}},
    {action::spawn_shadow, vector<int>{KEY_K}},
    {action::reset, vector<int>{KEY_BACKSPACE}},
};

bool inputs::is_action_pressed(action act) {
    for (auto button : inputs::action_buttons_[act]) {
        if (IsGamepadButtonPressed(0, button)) return true;
    }
    for (auto key : inputs::action_keys_[act]) {
        if (IsKeyPressed(key)) return true;
    }
    return false;
}

bool inputs::is_action_down(action act) {
    for (auto button : inputs::action_buttons_[act]) {
        if (IsGamepadButtonDown(0, button)) return true;
    }
    for (auto key : inputs::action_keys_[act]) {
        if (IsKeyDown(key)) return true;
    }
    return false;
}

bool inputs::is_action_up(action act) {
    return !(inputs::is_action_down(act));
}
