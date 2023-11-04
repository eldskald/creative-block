#pragma once
#include "game-element.h"

using namespace std;

class game {
public:
    static void do_tick_loop();
    static void set_root(game_element* new_root);
    static game_element* get_root();

private:
    static game_element* root_;
};
