#include "core/puff.h"
#include "core/game.h"
#include "defs.h"
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "imports.h"
#include <cmath>
#include <raylib.h>

using namespace std;

const float TAU = 2.0f * PI;
const int ITEN = 10;
const float FTEN = 10.0f;

puff::puff()
    : to_next_particle_(
          (float)GetRandomValue(PARTICLE_PUFF_MIN_SPAWN_TIME * ITEN,
                                PARTICLE_PUFF_MAX_SPAWN_TIME * ITEN) /
          FTEN) {
}

void puff::tick_() {
    this->to_next_particle_ -= GetFrameTime();
    if (this->to_next_particle_ <= 0.0f && this->free_) {
        this->free_ = false;
        this->spawn_particle_();
    }
}

void puff::spawn_particle_() {
    Vector2 init_pos = this->pos;
    init_pos.x += (float)GetRandomValue(0, SPRITESHEET_CELL_SIZE_X * 2) -
                  SPRITESHEET_CELL_SIZE_X;
    init_pos.y += (float)GetRandomValue(0, SPRITESHEET_CELL_SIZE_Y * 2) -
                  SPRITESHEET_CELL_SIZE_Y;
    auto particle = new puff::particle_(init_pos, this);
    game::get_root()->add_child(particle);
}

puff::particle_::particle_(Vector2 init_pos, puff* emitter)
    : emitter_(emitter),
      lifetime_((float)GetRandomValue(PARTICLE_PUFF_MIN_LIFETIME * ITEN,
                                      PARTICLE_PUFF_MAX_LIFETIME * ITEN) /
                FTEN) {
    this->pos = init_pos;
    this->type = physics_body::body_type::area;
    this->collision_mask = PARTICLE_PUFF_COLLISION_MASK;
    this->collision_box = PARTICLE_PUFF_COLLISION_BOX;
    auto* mask = new sprite();
    mask->atlas_coords = PARTICLE_PUFF_ATLAS_COORDS;
    mask->tint = MASK_BG_COLOR;
    mask->z_index = BACKGROUND_Z_INDEX;
    this->add_child(mask);
}

void puff::particle_::tick_() {
    this->time_ += GetFrameTime();
    this->vel.x = -cos(this->time_ * TAU / PARTICLE_PUFF_PERIOD) *
                  PARTICLE_PUFF_AMPLITUDE;
    this->vel.y = -cos(this->time_ * TAU / PARTICLE_PUFF_PERIOD) *
                      PARTICLE_PUFF_AMPLITUDE +
                  PARTICLE_PUFF_VERT_SPEED;
    if (this->time_ >= this->lifetime_) {
        this->emitter_->to_next_particle_ =
            (float)GetRandomValue(PARTICLE_PUFF_MIN_SPAWN_TIME * ITEN,
                                  PARTICLE_PUFF_MAX_SPAWN_TIME * ITEN) /
            FTEN;
        this->emitter_->free_ = true;
        this->mark_for_deletion();
        return;
    }
}
