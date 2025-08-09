#include "core/wisp.h"
#include "core/game.h"
#include "defs.h"
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "imports.h"
#include <raylib.h>

using namespace std;

const int ITEN = 10;
const float FTEN = 10.0f;

wisp::wisp()
    : to_next_particle_(
          (float)GetRandomValue(PARTICLE_WISP_MIN_SPAWN_TIME * ITEN,
                                PARTICLE_WISP_MAX_SPAWN_TIME * ITEN) /
          FTEN) {
}

void wisp::tick_() {
    if (this->is_marked_for_deletion()) return;

    this->to_next_particle_ -= GetFrameTime();
    if (this->to_next_particle_ <= 0.0f && this->free_) {
        this->free_ = false;
        this->spawn_particle_();
    }
}

void wisp::spawn_particle_() {
    Vector2 init_pos = this->pos;
    init_pos.x += (float)GetRandomValue(0, SPRITESHEET_CELL_SIZE_X * 2) -
                  SPRITESHEET_CELL_SIZE_X;
    init_pos.y += (float)GetRandomValue(0, SPRITESHEET_CELL_SIZE_Y * 2) -
                  SPRITESHEET_CELL_SIZE_Y;
    auto particle = new wisp::particle_(init_pos, this);
    game::get_root()->add_child(particle);
}

wisp::particle_::particle_(Vector2 init_pos, wisp* emitter)
    : emitter_(emitter) {
    this->pos = init_pos;
    this->type = physics_body::body_type::area;
    this->collision_mask = PARTICLE_WISP_COLLISION_MASK;
    this->collision_box = PARTICLE_WISP_COLLISION_BOX;
    this->vel.y = PARTICLE_WISP_FALL_SPEED;
    auto* mask = new sprite();
    mask->atlas_coords = PARTICLE_WISP_FRAME_1_ATLAS_COORDS;
    mask->tint = MASK_BG_COLOR;
    mask->z_index = BACKGROUND_Z_INDEX;
    int cycles =
        GetRandomValue(PARTICLE_WISP_MIN_CYCLES, PARTICLE_WISP_MAX_CYCLES);
    animation particle_anim;
    float lifetime = 0.0f;
    particle_anim.push_back((keyframe){PARTICLE_WISP_FRAME_1_ATLAS_COORDS,
                                       PARTICLE_WISP_FRAME_DURATION});
    lifetime += PARTICLE_WISP_FRAME_DURATION;
    for (int i = 0; i < cycles; i++) {
        particle_anim.push_back((keyframe){PARTICLE_WISP_FRAME_2_ATLAS_COORDS,
                                           PARTICLE_WISP_FRAME_DURATION});
        particle_anim.push_back((keyframe){PARTICLE_WISP_FRAME_3_ATLAS_COORDS,
                                           PARTICLE_WISP_FRAME_DURATION});
        lifetime += PARTICLE_WISP_FRAME_DURATION * 2;
    }
    particle_anim.push_back((keyframe){PARTICLE_WISP_FRAME_2_ATLAS_COORDS,
                                       PARTICLE_WISP_FRAME_DURATION});
    particle_anim.push_back((keyframe){PARTICLE_WISP_FRAME_1_ATLAS_COORDS,
                                       PARTICLE_WISP_FRAME_DURATION});
    lifetime += PARTICLE_WISP_FRAME_DURATION * 2;
    mask->anim = particle_anim;
    this->time_ = lifetime;
    this->add_child(mask);
}

void wisp::particle_::tick_() {
    this->time_ -= GetFrameTime();
    if (this->time_ <= 0.0f) {
        this->emitter_->to_next_particle_ =
            (float)GetRandomValue(PARTICLE_WISP_MIN_SPAWN_TIME * ITEN,
                                  PARTICLE_WISP_MAX_SPAWN_TIME * ITEN) /
            FTEN;
        this->emitter_->free_ = true;
        this->mark_for_deletion();
        return;
    }
}
