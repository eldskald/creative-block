#include "core/particle-effect.h"
#include "core/game.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

using namespace std;

const float THREE_DIGITS_PRECISION = 1000.0f;
const float BOUNCE_SPEED_LIMIT = 100.0f;

Vector2 particle_effect::get_one_starting_position_() {
    float x = (float)(rand() % (int)this->spawn_area.width) +
              this->get_global_pos().x + this->spawn_area.x;
    float y = (float)(rand() % (int)this->spawn_area.height) +
              this->get_global_pos().y + this->spawn_area.y;
    return (Vector2){x, y};
}

Vector2 particle_effect::get_one_starting_velocity_() {
    float dir_angle = this->direction_angle;
    if (this->spread_angle > 0) {
        dir_angle += (float)(rand() % (int)(THREE_DIGITS_PRECISION * 2 *
                                            this->spread_angle)) -
                     this->spread_angle;
    }
    float speed = this->speed;
    if (this->speed_variation > 0.0f) {
        speed *= 1.0f - ((float)(rand() % (int)(THREE_DIGITS_PRECISION *
                                                this->speed_variation)) /
                         THREE_DIGITS_PRECISION);
    }
    return Vector2Rotate((Vector2){speed, 0.0f}, (float)dir_angle);
}

float particle_effect::get_one_lifetime_() {
    float lifetime = this->lifetime;
    if (this->lifetime_variation > 0.0f) {
        lifetime *= 1.0f - ((float)(rand() % (int)(THREE_DIGITS_PRECISION *
                                                   this->lifetime_variation)) /
                            THREE_DIGITS_PRECISION);
    }
    return lifetime;
}

void particle_effect::emit() {
    for (int i = 0; i < this->particles; i++) {
        auto particle = new particle_effect::particle_();
        particle->lifetime = this->get_one_lifetime_();
        particle->pos = this->get_one_starting_position_();
        particle->vel = this->get_one_starting_velocity_();
        particle->prev_vel = particle->vel;
        particle->acceleration = this->acceleration;
        particle->elasticity_factor = this->elasticity_factor;
        particle->collision_box = this->particle_collision_box;
        particle->collision_mask = COL_MASK_PARTIC;
        particle->collision_layer = COL_LAYER_PARTIC;
        particle->mask_shader = this->mask_shader;

        int shader_rand_loc = GetShaderLocation(particle->mask_shader, "rand");
        int rand_value = rand() % (int)THREE_DIGITS_PRECISION;
        SetShaderValue(particle->mask_shader,
                       shader_rand_loc,
                       &rand_value,
                       SHADER_UNIFORM_INT);

        int shader_lifetime_loc =
            GetShaderLocation(particle->mask_shader, "lifetime");
        float lifetime_value = 1.0f;
        SetShaderValue(particle->mask_shader,
                       shader_lifetime_loc,
                       &lifetime_value,
                       SHADER_UNIFORM_FLOAT);

        auto mask = new sprite();
        mask->atlas_coords = this->mask_atlas_coords;
        mask->tint = this->mask_tint;
        particle->add_child(mask);
        game::get_root()->add_child(particle);
    }
}

void particle_effect::particle_::tick_() {
    this->relative_time_ -= GetFrameTime() / this->lifetime;
    if (this->relative_time_ <= 0.0f) {
        this->mark_for_deletion();
        return;
    }

    int shader_lifetime_loc = GetShaderLocation(this->mask_shader, "lifetime");
    float lifetime_value = this->relative_time_;
    SetShaderValue(this->mask_shader,
                   shader_lifetime_loc,
                   &lifetime_value,
                   SHADER_UNIFORM_FLOAT);

    if (this->collision_detected()) {
        if (!this->get_bodies_touching_top().empty() ||
            !this->get_bodies_touching_bottom().empty()) {
            this->vel.y = -(this->elasticity_factor * this->prev_vel.y);
            if (this->vel.y < 0.0f && this->vel.y > -BOUNCE_SPEED_LIMIT) {
                this->vel = Vector2Zero();
            }
        }
        if (!this->get_bodies_touching_left().empty() ||
            !this->get_bodies_touching_right().empty()) {
            this->vel.x = -(this->elasticity_factor * this->prev_vel.x);
        }
    }

    this->vel =
        Vector2Add(this->vel, Vector2Scale(this->acceleration, GetFrameTime()));
    this->prev_vel = this->vel;
}
