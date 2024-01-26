#pragma once
#include "game-element.h"
#include <raylib.h>

class game;

class sprite : public game_element {
    friend class game;

public:
    Vector2 atlas_coords{(Vector2){0}};
    Color tint{WHITE};
    Shader* shader{sprite::base_shader};

    static Shader* base_shader;

protected:
    void tick_() override;

private:
    static Texture2D atlas_;
};
