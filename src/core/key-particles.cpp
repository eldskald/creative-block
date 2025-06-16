#include "core/key-particles.h"
#include "core/game.h"
#include "defs.h"
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include <array>
#include <raylib.h>
#include <raymath.h>

const array<Vector2, 8> init_velocities = {
    PARTICLE_KEY_VEL_1,
    PARTICLE_KEY_VEL_2,
    PARTICLE_KEY_VEL_3,
    PARTICLE_KEY_VEL_4,
    PARTICLE_KEY_VEL_5,
    PARTICLE_KEY_VEL_6,
    PARTICLE_KEY_VEL_7,
    PARTICLE_KEY_VEL_8,
};

key_particles::particle_::particle_(Vector2 pos, Vector2 vel, Color tint)
    : prev_vel_(vel) {
    this->pos = pos;
    this->vel = vel;
    this->type = kinematic;
    this->collision_box = PARTICLE_KEY_COLLISION_BOX;
    this->collision_mask = PARTICLE_KEY_COLLISION_MASK;
    auto* mask = new sprite();
    mask->tint = tint;
    mask->atlas_coords = PARTICLE_KEY_ATLAS_COORDS;
    mask->z_index = BACKGROUND_Z_INDEX;
    this->add_child(mask);
}

void key_particles::particle_::tick_() {
    this->vel.y += PARTICLE_KEY_GRAVITY * GetFrameTime();

    bool collision_detected = false;
    if (!this->get_bodies_touching_left().empty() ||
        !this->get_bodies_touching_right().empty()) {
        this->vel.x = -PARTICLE_KEY_BOUNCE_ELASTICITY * this->prev_vel_.x;
        collision_detected = true;
    }
    if (!this->get_bodies_touching_top().empty() ||
        !this->get_bodies_touching_bottom().empty()) {
        this->vel.y = -PARTICLE_KEY_BOUNCE_ELASTICITY * this->prev_vel_.y;
        collision_detected = true;
    }
    if (collision_detected) {
        this->bounces_++;
        if (this->bounces_ >= PARTICLE_KEY_MAX_BOUNCES) {
            this->mark_for_deletion();
        }
    }

    this->prev_vel_ = this->vel;
}

void key_particles::emit() {
    Vector2 center = Vector2Add(this->get_global_pos(),
                                (Vector2){PARTICLE_KEY_COLLISION_BOX.x,
                                          PARTICLE_KEY_COLLISION_BOX.y});
    for (Vector2 vel : init_velocities) {
        game::get_root()->add_child(
            new key_particles::particle_(center, vel, this->tint));
    }
}
