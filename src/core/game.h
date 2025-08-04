#pragma once
#include "engine/game-element.h"
#include <raylib.h>

class game {
public:
#ifndef WEB
    static void close_game();
    static bool is_game_closing();
#endif

    static void on_start();
    static void on_close();
    static void do_game_loop();
    static void set_root(game_element* new_root);
    static game_element* get_root();
    static bool is_paused();
    static void pause();
    static void unpause();

private:
    static game_element* root_;
    static bool paused_;
#ifndef WEB
    static bool game_closing_;
#endif
};
