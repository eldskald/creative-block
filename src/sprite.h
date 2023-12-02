#pragma once
#include "game-element.h"
#include <raylib.h>

class game;

class sprite : public game_element {
    friend class game;

public:
    sprite();

    void set_texture(int x, int y);

private:
    Rectangle rec_;

    static Texture2D atlas_;

    void tick_() override;
};
