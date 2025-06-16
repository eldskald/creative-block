#include "core/water.h"
#include "core/base-unit.h"
#include "defs.h"
#include "engine/physics-body.h"
#include "engine/renderer.h"
#include "imports.h"
#include <raylib.h>
#include <raymath.h>

using namespace std;

water::water() {
    this->type = physics_body::body_type::area;
    this->collision_layer = COL_LAYER_WATER;
    this->collision_mask = COL_MASK_WATER;
}

void water::body_entered_(physics_body* body) {
    auto* unit_element = dynamic_cast<base_unit*>(body);
    if (unit_element) {
        unit_element->kill();
        const float two = 2.0f;
        renderer::add_water_wave(
            WATER_PLAYER_SPLASH_AMPLI,
            (unit_element->pos.x + SPRITESHEET_CELL_SIZE_X / two) /
                MAIN_TEX_SIZE,
            WATER_PLAYER_SPLASH_WIDTH);
        return;
    }
}

void water::area_entered_(physics_body* area) {
    renderer::add_water_wave(WATER_PARTICLE_SPLASH_AMPLI,
                             area->pos.x / MAIN_TEX_SIZE,
                             WATER_PARTICLE_SPLASH_WIDTH);
    area->mark_for_deletion();
}

void water::enter_() {
    physics_body::enter_();
    renderer::set_water(true, water_level);
    this->pos = Vector2Zero();
    this->collision_box =
        (Rectangle){0, this->water_level, WINDOW_SIZE_X, WINDOW_SIZE_Y};
}

void water::exit_() {
    physics_body::exit_();
    renderer::set_water(false, 0.0f);
}
