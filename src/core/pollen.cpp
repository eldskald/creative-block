#include "core/pollen.h"
#include "core/game.h"
#include "defs.h"
#include "engine/sprite.h"
#include <cmath>

const float TAU = 2.0f * PI;
const int ITEN = 10;
const float FTEN = 10.0f;

pollen::pollen()
    : to_next_particle_(
          (float)GetRandomValue(PARTICLE_POLLEN_MIN_SPAWN_TIME * ITEN,
                                PARTICLE_POLLEN_MAX_SPAWN_TIME * ITEN) /
          FTEN) {
}

void pollen::tick_() {
    if (this->is_marked_for_deletion()) return;

    this->to_next_particle_ -= GetFrameTime();
    if (this->to_next_particle_ <= 0.0f) {
        this->spawn_particle_();
        this->to_next_particle_ =
            (float)GetRandomValue(PARTICLE_POLLEN_MIN_SPAWN_TIME * ITEN,
                                  PARTICLE_POLLEN_MAX_SPAWN_TIME * ITEN) /
            FTEN;
    }
}

void pollen::spawn_particle_() {
    Vector2 init_pos = this->pos;
    init_pos.x += (float)GetRandomValue(0, SPRITESHEET_CELL_SIZE_X * 2) -
                  SPRITESHEET_CELL_SIZE_X;
    init_pos.y += (float)GetRandomValue(0, SPRITESHEET_CELL_SIZE_Y * 2) -
                  SPRITESHEET_CELL_SIZE_Y;
    auto particle = new pollen::particle_(init_pos);
    game::get_root()->add_child(particle);
}

pollen::particle_::particle_(Vector2 init_pos)
    : lifetime_((float)GetRandomValue(PARTICLE_POLLEN_MIN_LIFETIME * ITEN,
                                      PARTICLE_POLLEN_MAX_LIFETIME * ITEN) /
                FTEN) {
    this->pos = init_pos;
    this->type = physics_body::body_type::area;
    this->collision_mask = PARTICLE_POLLEN_COLLISION_MASK;
    this->collision_box = PARTICLE_POLLEN_COLLISION_BOX;
    auto* mask = new sprite();
    mask->atlas_coords = PARTICLE_POLLEN_ATLAS_COORDS;
    mask->tint = MASK_BG_COLOR;
    mask->z_index = BACKGROUND_Z_INDEX;
    this->add_child(mask);
}

void pollen::particle_::tick_() {
    this->time_ += GetFrameTime();
    this->vel.x = -cos(this->time_ * TAU / PARTICLE_POLLEN_PERIOD) *
                  PARTICLE_POLLEN_AMPLITUDE;
    this->vel.y = -cos(this->time_ * TAU / PARTICLE_POLLEN_PERIOD) *
                      PARTICLE_POLLEN_AMPLITUDE +
                  PARTICLE_POLLEN_VERT_SPEED;
    if (this->time_ >= this->lifetime_) {
        this->mark_for_deletion();
        return;
    }
}
