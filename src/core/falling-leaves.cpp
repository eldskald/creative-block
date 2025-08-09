#include "core/falling-leaves.h"
#include "core/game.h"
#include "defs.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "imports.h"
#include <cmath>
#include <raylib.h>

using namespace std;

const float TAU = 2.0f * PI;
const int ITEN = 10;
const float FTEN = 10.0f;

falling_leaves::falling_leaves()
    : to_next_particle_(
          (float)GetRandomValue(PARTICLE_LEAF_MIN_SPAWN_TIME * ITEN,
                                PARTICLE_LEAF_MAX_SPAWN_TIME * ITEN) /
          FTEN) {
}

void falling_leaves::tick_() {
    if (this->is_marked_for_deletion()) return;

    this->to_next_particle_ -= GetFrameTime();
    if (this->to_next_particle_ <= 0.0f && this->free_) {
        this->free_ = false;
        this->spawn_particle_();
    }
}

void falling_leaves::spawn_particle_() {
    Vector2 init_pos = this->pos;
    init_pos.x += (float)GetRandomValue(0, SPRITESHEET_CELL_SIZE_X * 2) -
                  SPRITESHEET_CELL_SIZE_X;
    init_pos.y += (float)GetRandomValue(0, SPRITESHEET_CELL_SIZE_Y * 2) -
                  SPRITESHEET_CELL_SIZE_Y;
    auto particle = new falling_leaves::particle_(init_pos, this);
    game::get_root()->add_child(particle);
}

falling_leaves::particle_::particle_(Vector2 init_pos, falling_leaves* emitter)
    : emitter_(emitter) {
    this->pos = init_pos;
    this->type = physics_body::body_type::area;
    this->collision_mask = PARTICLE_LEAF_COLLISION_MASK;
    this->collision_box = PARTICLE_LEAF_COLLISION_BOX;
    this->collision_layer = COL_LAYER_PARTICLES;
    auto* mask = new sprite();
    mask->atlas_coords = PARTICLE_LEAF_ATLAS_COORDS;
    mask->tint = MASK_BG_COLOR;
    mask->z_index = BACKGROUND_Z_INDEX;
    this->add_child(mask);
}

void falling_leaves::particle_::tick_() {
    if (!this->get_detected_bodies().empty()) {
        this->emitter_->to_next_particle_ =
            (float)GetRandomValue(PARTICLE_LEAF_MIN_SPAWN_TIME * ITEN,
                                  PARTICLE_LEAF_MAX_SPAWN_TIME * ITEN) /
            FTEN;
        this->emitter_->free_ = true;
        this->mark_for_deletion();
        return;
    }
    this->time_ += GetFrameTime();
    this->vel.x = -cos(this->time_ * TAU / PARTICLE_LEAF_SWAY_PERIOD) *
                      PARTICLE_LEAF_SWAY_AMPLITUDE_X +
                  PARTICLE_LEAF_BASE_SPEED_X;
    this->vel.y = -cos(this->time_ * TAU / PARTICLE_LEAF_SWAY_PERIOD) *
                      PARTICLE_LEAF_SWAY_AMPLITUDE_Y +
                  PARTICLE_LEAF_BASE_SPEED_Y;
}
