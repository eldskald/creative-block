#pragma once
#include "game-element.h"
#include <raylib.h>

class sprite : public game_element {
public:
    sprite();

    void set_texture(const char* file);

private:
    Texture2D texture_;

    void tick_() override;
};
