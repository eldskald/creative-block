#pragma once
#include "defs.h"
#include <raylib.h>
#include <string>

using namespace std;

class button {
public:
    Rectangle rect{(Rectangle){0}};
    string label{""};
    void (*on_click)(){button::empty_on_click_};

    void tick();

private:
    bool being_clicked_{false};

    void render_();
    void detect_clicks_();
    bool is_being_hovered_();

    static void empty_on_click_();
};
