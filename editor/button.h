#pragma once
#include "defs.h"
#include <raylib.h>
#include <string>

using namespace std;

class button {
public:
    Rectangle rect{(Rectangle){0}};
    string label{""};

    void tick();

private:
    void render_();
    bool is_being_hovered_();
};
