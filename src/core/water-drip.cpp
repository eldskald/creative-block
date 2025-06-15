#include "core/water-drip.h"
#include "core/game.h"
#include "core/water.h"
#include "defs.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "imports.h"
#include <raylib.h>
#include <raymath.h>

using namespace std;

const int ITEN = 10;
const float FTEN = 10.0f;

water_drip::water_drip()
    : to_next_particle_(
          (float)GetRandomValue(ITEN, PARTICLE_DRIP_SPAWN_TIME * ITEN) / FTEN) {
    this->atlas_coords = PARTICLE_DRIP_FRAME_1_ATLAS_COORDS;
    this->tint = MASK_BG_COLOR;
}

void water_drip::tick_() {
    this->to_next_particle_ -= GetFrameTime();
    if (this->to_next_particle_ <= PARTICLE_DRIP_FRAME_2_TIME) {
        this->atlas_coords = PARTICLE_DRIP_FRAME_2_ATLAS_COORDS;
    } else {
        this->atlas_coords = PARTICLE_DRIP_FRAME_1_ATLAS_COORDS;
    }
    if (this->to_next_particle_ <= 0.0f) {
        this->spawn_particles_();
        this->to_next_particle_ = PARTICLE_DRIP_SPAWN_TIME;
    }
}

void water_drip::spawn_particles_() {
    auto* drop = new water_drip::particle_(this->pos);
    game::get_root()->add_child(drop);
}

water_drip::particle_::particle_(Vector2 pos) {
    this->type = physics_body::body_type::area;
    this->collision_box = PARTICLE_DRIP_PARTICLE_COLLISION_BOX;
    this->collision_mask = PARTICLE_DRIP_COLLISION_MASK;
    this->collision_layer = COL_LAYER_PARTICLES;
    this->pos = pos;
    auto* drop_sprite = new sprite();
    drop_sprite->atlas_coords = PARTICLE_DRIP_PARTICLE_ATLAS_COORDS;
    drop_sprite->tint = MASK_BG_COLOR;
    drop_sprite->z_index = BACKGROUND_Z_INDEX;
    this->add_child(drop_sprite);
}

water_drip::sub_particle_::sub_particle_(Vector2 pos, Vector2 vel) {
    this->type = physics_body::body_type::area;
    this->collision_box = PARTICLE_DRIP_SUB_PARTICLE_COLLISION_BOX;
    this->collision_mask = PARTICLE_DRIP_COLLISION_MASK;
    this->collision_layer = COL_LAYER_PARTICLES;
    this->pos = pos;
    this->vel = vel;
    auto* drop_sprite = new sprite();
    drop_sprite->atlas_coords = PARTICLE_DRIP_SUB_PARTICLE_ATLAS_COORDS;
    drop_sprite->tint = MASK_BG_COLOR;
    drop_sprite->z_index = BACKGROUND_Z_INDEX;
    this->add_child(drop_sprite);
}

void water_drip::particle_::tick_() {
    if (!this->get_detected_bodies().empty()) {
        this->mark_for_deletion();
        this->spawn_sub_particles_();
        return;
    }
    if (!this->get_detected_areas().empty()) {
        for (auto body : this->get_detected_bodies()) {
            auto* water_body = dynamic_cast<water*>(body);
            if (water_body) {
                this->mark_for_deletion();
                return;
            }
        }
    }
    this->vel.y += PARTICLE_DRIP_GRAVITY * GetFrameTime();
    if (this->vel.y > PARTICLE_DRIP_MAX_FALLING_SPEED) {
        this->vel.y = PARTICLE_DRIP_MAX_FALLING_SPEED;
    }
}

void water_drip::sub_particle_::tick_() {
    if (!this->get_detected_bodies().empty() ||
        !this->get_detected_areas().empty()) {
        this->mark_for_deletion();
        return;
    }
    this->vel.y += PARTICLE_DRIP_GRAVITY * GetFrameTime();
    if (this->vel.y > PARTICLE_DRIP_MAX_FALLING_SPEED) {
        this->vel.y = PARTICLE_DRIP_MAX_FALLING_SPEED;
    }
}

void water_drip::particle_::spawn_sub_particles_() {
    auto* drop_1 = new water_drip::sub_particle_(
        Vector2Add(this->pos, (Vector2){0.0f, -SPRITESHEET_CELL_SIZE_Y}),
        PARTICLE_DRIP_SUB_PARTICLE_VEL_1);
    auto* drop_2 = new water_drip::sub_particle_(
        Vector2Add(this->pos, (Vector2){0.0f, -SPRITESHEET_CELL_SIZE_Y}),
        PARTICLE_DRIP_SUB_PARTICLE_VEL_2);
    auto* drop_3 = new water_drip::sub_particle_(
        Vector2Add(this->pos, (Vector2){0.0f, -SPRITESHEET_CELL_SIZE_Y}),
        PARTICLE_DRIP_SUB_PARTICLE_VEL_3);
    auto* drop_4 = new water_drip::sub_particle_(
        Vector2Add(this->pos, (Vector2){0.0f, -SPRITESHEET_CELL_SIZE_Y}),
        PARTICLE_DRIP_SUB_PARTICLE_VEL_4);
    game::get_root()->add_child(drop_1);
    game::get_root()->add_child(drop_2);
    game::get_root()->add_child(drop_3);
    game::get_root()->add_child(drop_4);
}
