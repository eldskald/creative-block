#include "core/base-unit.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>

using namespace std;

void base_unit::change_dir(Vector2 dir) {
    this->dir_ = dir;
}

void base_unit::press_jump() {
    if (this->jump_buffer_timer_ <= PLAYER_JUMP_BUFFER && !this->is_jumping_) {
        this->vel.y = -PLAYER_JUMP_SPEED;
        this->is_jumping_ = true;
    }
}

void base_unit::release_jump() {
    if (this->is_jumping_) {
        this->vel.y *= PLAYER_JUMP_SPEED_CUT;
        this->is_jumping_ = false;
    }
}

Vector2 base_unit::get_dir() {
    return this->dir_;
}

bool base_unit::is_grounded() {
    return !this->get_bodies_touching_bottom().empty();
}

void base_unit::tick_() {

    // Accelerate left/right according to dir input
    this->vel.x = clamp(this->vel.x + PLAYER_ACCELERATION * GetFrameTime() *
                                          (this->dir_.x),
                        -PLAYER_SPEED,
                        PLAYER_SPEED);

    // Accelerate the other way in case there is no dir input or the player
    // switched dir input
    if ((this->vel.x * this->dir_.x == -1 or this->dir_.x == 0) &&
        this->vel.x != 0.0f) {
        if (fabs(this->vel.x) < PLAYER_FRICTION) {
            this->vel.x = 0.0f;
        } else if (this->vel.x > 0.0f) {
            this->vel.x -= PLAYER_FRICTION * GetFrameTime();
        } else {
            this->vel.x += PLAYER_FRICTION * GetFrameTime();
        }
    }

    // Gravity
    this->vel.y += PLAYER_GRAVITY * GetFrameTime();
    if (this->vel.y > PLAYER_MAX_FALL_SPEED) {
        this->vel.y = PLAYER_MAX_FALL_SPEED;
    }

    // Jump buffer. If it's smaller than the limit, you can jump
    this->jump_buffer_timer_ += GetFrameTime();
    if (this->is_grounded()) {
        this->jump_buffer_timer_ = 0.0f;
    }
    if (this->is_jumping_) {
        this->jump_buffer_timer_ = PLAYER_JUMP_BUFFER + 1.0f;
    }

    // End jumping state if you reached peak of the jump
    if (this->is_jumping_ && this->vel.y >= 0.0f) {
        this->is_jumping_ = false;
    }

    // The code that cause the jump is on the press_jump() function, and the
    // code that causes a jump to end when you release the button is on the
    // release_jump() function.
}
