#pragma once
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include <raylib.h>

using namespace std;

class particle_effect : public game_element {
public:
    int particles{0};
    float lifetime{0.0f};
    float lifetime_variation{0.0f};
    Vector2 mask_atlas_coords{(Vector2){0}};
    Color mask_tint{(Color){0}};
    Shader mask_shader{(Shader){0}};
    float speed{0.0f};
    float speed_variation{0.0f};
    Vector2 acceleration{(Vector2){0}};
    Rectangle spawn_area{(Rectangle){0}};
    float direction_angle{0};
    float spread_angle{0};
    Rectangle particle_collision_box{(Rectangle){0}};
    float elasticity_factor{0.0f};

    void emit();

private:
    class particle_ : public physics_body {
    public:
        float lifetime{0.0f};
        Vector2 acceleration{(Vector2){0}};
        Vector2 prev_vel{(Vector2){0}};
        float elasticity_factor{0.0f};
        Shader mask_shader{(Shader){0}};

    protected:
        void tick_() override;

    private:
        float relative_time_{1.0f};
    };

    Vector2 get_one_starting_position_();
    Vector2 get_one_starting_velocity_();
    float get_one_lifetime_();
};
