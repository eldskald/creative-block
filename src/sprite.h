#pragma once
#include "game-element.h"
#include <raylib.h>

class game;

class sprite : public game_element {
    friend class game;

public:
    sprite();

    Vector2 atlas_coords;
    Shader* shader;

    static Shader* base_shader;

protected:
    void tick_() override;

private:
    static Texture2D atlas_;
};
