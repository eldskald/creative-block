#pragma once
#include "game-element.h"
#include <list>
#include <raylib.h>

using namespace std;

class physics_body : public game_element {
public:
    Vector2 vel;
    Rectangle collision_box;

    static list<physics_body*> get_colliders(Rectangle collision_box);

    physics_body();

private:
    void update_pos_();

    static list<physics_body*> physics_bodies_;

    void enter_() override;
    void exit_() override;
    void tick_() override;
};
