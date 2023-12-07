#include "sprite.h"
#include "defs.h"
#include <raylib.h>
#include <vector>

using namespace std;

Texture2D sprite::atlas_ = {0};

sprite::sprite() {
    this->atlas_rec_ =
        (Rectangle){0, 0, SPRITESHEET_CELL_SIZE_X, SPRITESHEET_CELL_SIZE_Y};
    this->curr_anim_time_ = 0.0f;
    this->curr_anim_frame_ = 0;
}

void sprite::set_texture(int x, int y) {
    this->atlas_rec_ = (Rectangle){SPRITESHEET_CELL_SIZE_X * x,
                                   SPRITESHEET_CELL_SIZE_Y * y,
                                   SPRITESHEET_CELL_SIZE_X,
                                   SPRITESHEET_CELL_SIZE_Y};
}

void sprite::set_animation(vector<defs::animation_frame> animation) {
    this->animation_ = animation;
    this->curr_anim_time_ = 0.0f;
    if (animation.size() > 0) {
        this->curr_anim_frame_ = 0;
        this->set_texture(animation[0].x, animation[0].y);
    }
    this->animation_.shrink_to_fit();
}

void sprite::tick_() {
    if (this->animation_.size() > 0) {
        this->curr_anim_time_ += GetFrameTime();
        if (this->curr_anim_time_ >=
            this->animation_[this->curr_anim_frame_].duration) {
            this->curr_anim_time_ -=
                this->animation_[this->curr_anim_frame_].duration;
            this->curr_anim_frame_ =
                (this->curr_anim_frame_ + 1) % this->animation_.size();
            this->set_texture(this->animation_[this->curr_anim_frame_].x,
                              this->animation_[this->curr_anim_frame_].y);
        }
    }
    DrawTexturePro(sprite::atlas_,
                   this->atlas_rec_,
                   (Rectangle){this->global_pos_.x,
                               this->global_pos_.y,
                               SPRITESHEET_CELL_SIZE_X * PIXEL_SIZE_X,
                               SPRITESHEET_CELL_SIZE_Y * PIXEL_SIZE_Y},
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
}

void sprite::enter_() {
}

void sprite::exit_() {
}
