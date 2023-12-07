#pragma once
#include "game-element.h"
#include <raylib.h>

class game {
public:
    static void initial_setup();
    static void do_game_loop();
    static void set_root(game_element* new_root);
    static game_element* get_root();

private:
    static game_element* root_;
    static void tick_game_element_(game_element* element);
    static void update_game_element_pos_(game_element* element);
};
