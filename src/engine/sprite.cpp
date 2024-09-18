#include "sprite.h"
#include <raylib.h>
#include <cmath>

Texture2D sprite::atlas_ = (Texture2D){0};
list<sprite*> sprite::sprites_;

void sprite::initialize() {
    sprite::atlas_ = LoadTexture(SPRITESHEET_FILE);
    SetTextureWrap(sprite::atlas_, TEXTURE_WRAP_CLAMP);
}

void sprite::enter_() {
    this->curr_phase_ = this->animation_starting_phase;
    sprite::sprites_.push_back(this);
}

void sprite::exit_() {
    sprite::sprites_.remove(this);
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
}

void sprite::render_sprites_() {
    for (auto sprite : sprite::sprites_) {
        DrawTexturePro(
            sprite::atlas_,
            (Rectangle){SPRITESHEET_CELL_SIZE_X * sprite->atlas_coords.x,
                        SPRITESHEET_CELL_SIZE_Y * sprite->atlas_coords.y,
                        SPRITESHEET_CELL_SIZE_X,
                        SPRITESHEET_CELL_SIZE_Y},
            (Rectangle){floor(sprite->get_global_pos().x),
                        floor(sprite->get_global_pos().y),
                        SPRITESHEET_CELL_SIZE_X,
                        SPRITESHEET_CELL_SIZE_Y},
            (Vector2){0, 0},
            0.0f,
            sprite->tint);
    }
}
