#pragma once
#include "game-element.h"

using namespace std;

class Game {
public:
    static void do_tick_loop();
    static void set_root(GameElement* new_root);
    static GameElement* get_root();

private:
    static GameElement* _root;
};
