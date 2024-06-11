#include "sprite.h"
#include <raylib.h>

Texture2D sprite::atlas_ = (Texture2D){0};

void sprite::initialize() {
    sprite::atlas_ = LoadTexture(SPRITESHEET_FILE);
}

void sprite::tick_() {
    BeginShaderMode(*(this->shader));
    DrawTexturePro(sprite::atlas_,
                   (Rectangle){SPRITESHEET_CELL_SIZE_X * this->atlas_coords.x,
                               SPRITESHEET_CELL_SIZE_Y * this->atlas_coords.y,
                               SPRITESHEET_CELL_SIZE_X,
                               SPRITESHEET_CELL_SIZE_Y},
                   (Rectangle){this->get_global_pos().x,
                               this->get_global_pos().y,
                               SPRITESHEET_CELL_SIZE_X,
                               SPRITESHEET_CELL_SIZE_Y},
                   (Vector2){0, 0},
                   0.0f,
                   this->tint);
    EndShaderMode();
}
