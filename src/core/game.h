#pragma once
#include "engine/game-element.h"
#include <raylib.h>

class game {
public:
    static void on_start();
    static void on_close();
    static void do_game_loop();
    static void set_root(game_element* new_root);
    static game_element* get_root();

private:
    static game_element* root_;
};
