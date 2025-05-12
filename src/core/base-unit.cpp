#include "core/base-unit.h"
#include "core/game.h"
#include "defs.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <raymath.h>

using namespace std;

void base_unit::change_dir(Vector2 dir) {
    this->dir_ = dir;
}

bool base_unit::press_jump() {
    if (this->jump_buffer_timer_ <= PLAYER_JUMP_BUFFER && !this->is_jumping_) {
        this->vel.y = -PLAYER_JUMP_SPEED;
        this->is_jumping_ = true;
        this->is_pre_buffering_jump_ = false;
        return true;
    } else {
        this->is_pre_buffering_jump_ = true;
        this->jump_buffer_timer_ = 0.0f;
        return false;
    }
}

void base_unit::release_jump() {
    this->is_pre_buffering_jump_ = false;
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

void base_unit::kill() {
    return;
}

void base_unit::tick_() {

    // Brake in case there is no dir input or the player is going the other way
    if (this->vel.x * this->dir_.x <= 0.0f && this->vel.x != 0.0f) {
        float sign = this->vel.x > 0.0f ? -1.0f : 1.0f;
        this->vel.x += PLAYER_FRICTION * GetFrameTime() * sign;
    }

    // Stop the player from jittering after it stops moving
    if (this->dir_.x == 0.0f && this->vel.x != 0.0f &&
        fabs(this->vel.x) < PLAYER_FRICTION * GetFrameTime()) {
        this->vel.x = 0.0f;
    }

    // Accelerate left/right according to dir input
    this->vel.x = clamp(this->vel.x + PLAYER_ACCELERATION * GetFrameTime() *
                                          (this->dir_.x),
                        -PLAYER_SPEED,
                        PLAYER_SPEED);

    // Gravity. Can't clamp because there's no max vertical rising speed
    this->vel.y += PLAYER_GRAVITY * GetFrameTime();
    if (this->vel.y > PLAYER_MAX_FALL_SPEED) {
        this->vel.y = PLAYER_MAX_FALL_SPEED;
    }

    // Jump buffer. It's supposed to let you pressed jump just before touching
    // the ground and still let you jump, as well as let you press jump right
    // after just running off an edge and still jump.
    this->jump_buffer_timer_ += GetFrameTime();
    if (this->is_pre_buffering_jump_ &&
        this->jump_buffer_timer_ <= PLAYER_JUMP_BUFFER && this->is_grounded()) {
        this->vel.y = -PLAYER_JUMP_SPEED;
        this->is_jumping_ = true;
        this->is_pre_buffering_jump_ = false;
    }
    if (this->is_grounded()) {
        this->jump_buffer_timer_ = 0.0f;
        this->is_pre_buffering_jump_ = false;
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

    // Checks if it is on top of another base_unit to see if it gets carried
    bool is_carried = false;
    for (auto* body : this->get_bodies_touching_bottom()) {
        auto* unit = dynamic_cast<base_unit*>(body);
        if (unit) {
            is_carried = true;
            if (this->get_parent() != unit) this->reparent(unit);
            // We do this next line because reparenting happens at the end of
            // the tick cycle, and moves the child's pos to match it's pos on
            // screen in relation to its new parent. The point is to fix the y
            // pos to eliminate fraction values because they were accumulating
            // and would cause the body to fall off its parent when it moved
            // down, usually after a jump for example.
            else
                this->pos.y = -this->collision_box.height;
            break;
        }
    }
    if (!is_carried && this->get_parent() != game::get_root()) {
        this->reparent(game::get_root());
    }
}
