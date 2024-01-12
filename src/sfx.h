#pragma once
#include "game-element.h"
#include <raylib.h>

class sfx : public game_element {
public:
    sfx(Wave sound);
    ~sfx();

    void play();
    void stop();

    static Wave sfx_1;
    static Wave sfx_2;

private:
    Sound sound_;
};
