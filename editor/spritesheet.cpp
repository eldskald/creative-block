#include "spritesheet.h"
#include "defs.h"
#include <raylib.h>

Texture2D spritesheet::spritesheet_ = (Texture2D){0};

void spritesheet::initialize() {
    spritesheet::spritesheet_ = LoadTexture(SPRITESHEET_PATH);
}

void spritesheet::render_sprite_at(Vector2 sprite_coords, Vector2 at, Color tint) {
    DrawTexturePro(spritesheet::spritesheet_,
                   (Rectangle){SPRITESHEET_CELL_X * sprite_coords.x,
                               SPRITESHEET_CELL_Y * sprite_coords.y,
                               SPRITESHEET_CELL_X,
                               SPRITESHEET_CELL_Y},
                   (Rectangle){at.x, at.y, CELL_SIZE_X, CELL_SIZE_Y},
                   (Vector2){0, 0},
                   0.0f,
                   tint);
}
