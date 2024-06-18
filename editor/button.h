#pragma once
#include <raylib.h>
#include <string>

using namespace std;

class button {
public:
    Rectangle rect{(Rectangle){0}};
    string label{""};
    bool toggle_mode{false};
    void (*on_click)(){button::empty_on_click_};

    bool is_pressed();
    void set_toggle(bool value);

    void tick();

private:
    bool being_clicked_{false};
    bool pressed_{false};

    void render_();
    void detect_clicks_();
    bool is_being_hovered_();

    static void empty_on_click_();
};
