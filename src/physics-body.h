#pragma once
#include "game-element.h"
#include <list>
#include <raylib.h>

using namespace std;

class physics_body : public game_element {
public:
    Vector2 vel;
    Rectangle collision_box;
    bool static_body;

    static list<physics_body*> get_colliders(Rectangle collision_box);

    physics_body();

protected:
    void update_pos_() override;

private:
    static list<physics_body*> physics_bodies_;

    void enter_() override;
    void exit_() override;
};
