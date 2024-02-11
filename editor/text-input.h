#pragma once
#include <raylib.h>
#include <string>
#include <vector>

using namespace std;

class text_input {
public:
    Rectangle rect{(Rectangle){0}};
    string label{""};

    string get_input();
    void tick();

private:
    bool focused_{false};
    vector<int> input_codepoints_{};
    string input_text_{""};
    vector<float> input_lengths_{0.0};
    int cursor_pos_{0};
    float cursor_blink_time_{0.0};
    float hold_key_start_{0.0};
    float hold_key_period_{0.0};

    void render_();
    void detect_clicks_();
    bool is_being_hovered_();
    void get_user_input_();
    void update_inputs_();

    static auto hold_key_map_(int key) -> void (*)(text_input*);
    static void erase_back_(text_input* input);
    static void move_cursor_left_(text_input* input);
    static void move_cursor_right_(text_input* input);
    static void move_cursor_home_(text_input* input);
    static void move_cursor_end_(text_input* input);
};
