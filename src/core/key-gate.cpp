#include "core/key-gate.h"
#include "defs.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "imports.h"
#include <list>
#include <raylib.h>
#include <raymath.h>

const float GATE_WIDTH = SPRITESHEET_CELL_SIZE_X - 2.0f;
const float GATE_REC_LINE_WIDTH = 2.0f;
const Vector2 GATE_SPRITE_ATLAS_COORDS = (Vector2){7, 5};
const animation KEY_ANIM = {
    (keyframe){{8, 4}, 0.2},
    (keyframe){{8, 5}, 0.2},
    (keyframe){{8, 6}, 0.2},
    (keyframe){{8, 7}, 0.2},
    (keyframe){{9, 4}, 0.2},
    (keyframe){{9, 5}, 0.2},
    (keyframe){{9, 6}, 0.2},
    (keyframe){{9, 5}, 0.2},
    (keyframe){{9, 4}, 0.2},
    (keyframe){{8, 7}, 0.2},
    (keyframe){{8, 6}, 0.2},
    (keyframe){{8, 5}, 0.2},
};

list<key_gate*> key_gate::key_gates_;

key_gate::key_::key_(key_gate* gate) : gate_(gate), key_sprite_(new sprite()) {
    this->key_sprite_->anim = KEY_ANIM;
    this->key_sprite_->tint = MASK_MAIN_COLOR;
    this->add_child(this->key_sprite_);
    this->collision_box =
        (Rectangle){0, 0, SPRITESHEET_CELL_SIZE_X, SPRITESHEET_CELL_SIZE_Y};
    this->type = physics_body::area;
    this->collision_mask = KEY_GATE_KEY_COLLISION_MASK;
}

bool key_gate::key_::is_active() {
    return this->active_;
}

void key_gate::key_::enable_() {
    this->active_ = true;
    this->collision_mask = KEY_GATE_KEY_COLLISION_MASK;
    this->key_sprite_->hidden = false;
}

void key_gate::key_::body_entered_(physics_body* body) {
    this->active_ = false;
    this->collision_mask = 0b00000000;
    this->key_sprite_->hidden = true;
    this->gate_->key_gotten_();
}

void key_gate::key_::on_player_death_() {
    this->enable_();
}

void key_gate::key_::on_player_shadow_() {
    this->enable_();
}

key_gate::key_gate() : gate_collider_(new physics_body()) {
    this->collision_box = (Rectangle){
        0.0f, 0.0f, SPRITESHEET_CELL_SIZE_X, SPRITESHEET_CELL_SIZE_Y};
    this->collision_layer = KEY_GATE_GATE_COLLISION_LAYER;
    this->type = physics_body::fixed;
    auto gate_sprite = new sprite();
    gate_sprite->atlas_coords = GATE_SPRITE_ATLAS_COORDS;
    gate_sprite->tint = MASK_MAIN_COLOR;
    this->add_child(gate_sprite);
    this->gate_collider_->type = physics_body::fixed;
    this->gate_collider_->collision_layer = KEY_GATE_GATE_COLLISION_LAYER;
    this->gate_collider_->collision_box =
        (Rectangle){1.0f, SPRITESHEET_CELL_SIZE_Y, GATE_WIDTH, 0.0f};
    this->add_child(this->gate_collider_);
}

void key_gate::enter_() {
    physics_body::enter_();
    for (Vector2 pos : this->key_positions) {
        key_* key = new key_(this);
        key->pos = Vector2Subtract(pos, this->pos);
        this->keys_.push_back(key);
        this->add_child(key);
    }
    this->current_height_ = this->height;
    key_gate::key_gates_.push_back(this);
}

void key_gate::exit_() {
    key_gate::key_gates_.remove(this);
}

void key_gate::on_player_death_() {
    this->enable_();
}

void key_gate::on_player_shadow_() {
    this->enable_();
}

void key_gate::key_gotten_() {
    for (key_* key : this->keys_) {
        if (key->is_active()) return;
    }
    this->active_ = false;
}

void key_gate::enable_() {
    this->active_ = true;
    this->current_height_ = this->height;
}

void key_gate::tick_() {
    if (!this->active_ && this->current_height_ > 0.0f) {
        this->current_height_ = Clamp(
            this->current_height_ - KEY_GATE_OPENING_SPEED * GetFrameTime(),
            0.0f,
            this->height);
    }
    this->gate_collider_->collision_box.height = this->current_height_;
}

void key_gate::render_gates_() {
    for (key_gate* gate : key_gate::key_gates_) {
        DrawRectangleLinesEx(
            (Rectangle){
                gate->pos.x + 1.0f,
                gate->pos.y + SPRITESHEET_CELL_SIZE_Y,
                GATE_WIDTH,
                gate->current_height_,
            },
            GATE_REC_LINE_WIDTH,
            MASK_MAIN_COLOR);
    }
}
