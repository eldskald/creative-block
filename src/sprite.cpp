#include "sprite.h"
#include "settings.h"
#include <raylib.h>
#include <raymath.h>

Texture2D sprite::atlas_ = {0};

sprite::sprite() {
    this->rec_ = (Rectangle){0,
                             0,
                             game_settings::SPRITESHEET_CELL_SIZE_X,
                             game_settings::SPRITESHEET_CELL_SIZE_Y};
}

void sprite::set_texture(int x, int y) {
    this->rec_ = (Rectangle){game_settings::SPRITESHEET_CELL_SIZE_X * x,
                             game_settings::SPRITESHEET_CELL_SIZE_Y * y,
                             game_settings::SPRITESHEET_CELL_SIZE_X,
                             game_settings::SPRITESHEET_CELL_SIZE_Y};
}

void sprite::tick_() {
    DrawTexturePro(sprite::atlas_,
                   this->rec_,
                   (Rectangle){
                       this->global_pos_.x,
                       this->global_pos_.y,
                       game_settings::SPRITESHEET_CELL_SIZE_X *
                           game_settings::PIXEL_SIZE_X,
                       game_settings::SPRITESHEET_CELL_SIZE_Y *
                           game_settings::PIXEL_SIZE_Y,
                   },
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
}


