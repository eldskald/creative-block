#pragma once
#include "game-element.h"
#include <list>
#include <raylib.h>

using namespace std;

class game;

class physics_body : public game_element {
    friend class game;

public:
    Vector2 vel;
    Rectangle collision_box;
    bool static_body;
    bool one_way;

    static list<physics_body*> get_colliders(Rectangle collision_box);

    physics_body();

protected:
    void physics_tick_();

    void enter_() override;
    void exit_() override;

private:
    Vector2 prev_global_pos_;

    float compute_h_movement_(float delta_d);
    float compute_v_movement_(float delta_d);
    Vector2 get_extrusion_from_(Rectangle rect);

    static list<physics_body*> physics_bodies_;

    static void trigger_physics_tick_(game_element* element);
    static void update_all_bodies_prev_global_pos_();
};
