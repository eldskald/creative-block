#pragma once
#include <raylib.h>

class spritesheet {
public:
    static void initialize();
    static void render_sprite_at(Vector2 sprite_coords, Vector2 at, Color tint);

private:
    static Texture2D spritesheet_;
};
