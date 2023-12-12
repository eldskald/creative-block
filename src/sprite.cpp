#include "sprite.h"
#include <raylib.h>

Texture2D sprite::atlas_ = {0};

sprite::sprite() {
    this->atlas_coords = (Vector2){0, 0};
}

void sprite::tick_() {
    DrawTexturePro(sprite::atlas_,
                   (Rectangle){SPRITESHEET_CELL_SIZE_X * this->atlas_coords.x,
                               SPRITESHEET_CELL_SIZE_Y * this->atlas_coords.y,
                               SPRITESHEET_CELL_SIZE_X,
                               SPRITESHEET_CELL_SIZE_Y},
                   (Rectangle){this->global_pos_.x,
                               this->global_pos_.y,
                               SPRITESHEET_CELL_SIZE_X * PIXEL_SIZE_X,
                               SPRITESHEET_CELL_SIZE_Y * PIXEL_SIZE_Y},
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
}
