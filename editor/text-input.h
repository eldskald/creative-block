#pragma once
#include <raylib.h>
#include <string>

using namespace std;

class text_input {
public:
    Rectangle rect{(Rectangle){0}};
    string label{""};

    string get_input();
    void tick();

private:
    bool focused_{false};
    string input_{""};
    float cursor_blink_time_{0.0};

    void render_();
    void detect_clicks_();
    bool is_being_hovered_();
    void update_input_();
};
