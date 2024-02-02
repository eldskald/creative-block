#pragma once
#include "game-element.h"
#include "shader.h"
#include <raylib.h>

class sprite : public game_element {
public:
    Vector2 atlas_coords{(Vector2){0}};
    Color tint{WHITE};
    Shader* shader{shader::get_base()};

    static Shader* base_shader;

    static void initialize();

protected:
    void tick_() override;

private:
    static Texture2D atlas_;
};
