#include "sprite.h"
#include <raylib.h>

Texture2D sprite::atlas_ = (Texture2D){0};

void sprite::initialize() {
    sprite::atlas_ = LoadTexture(SPRITESHEET_FILE);
}

void sprite::enter_() {
    this->curr_phase_ = this->animation_starting_phase;
}

void sprite::tick_() {
    if (this->anim.size() > 0) {
        this->curr_phase_ += GetFrameTime();
        while (this->curr_phase_ >= this->anim.at(this->curr_frame_).duration) {
            this->curr_phase_ -= this->anim.at(this->curr_frame_).duration;
            this->curr_frame_ =
                (this->curr_frame_ + 1) % (int)this->anim.size();
            this->atlas_coords = this->anim.at(this->curr_frame_).atlas_coords;
        }
    }
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
