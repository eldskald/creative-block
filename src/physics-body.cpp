#include "physics-body.h"
#include <list>
#include <raylib.h>

using namespace std;

list<physics_body*> physics_body::physics_bodies_;

physics_body::physics_body() {
    this->vel = (Vector2){0};
    this->collision_box = (Rectangle){0};
}

list<physics_body*> physics_body::get_colliders(Rectangle collision_box) {
    list<physics_body*> colliders;
    for (auto i = physics_body::physics_bodies_.begin();
         i != physics_body::physics_bodies_.end();
         ++i) {
        if (CheckCollisionRecs(collision_box, (*i)->collision_box)) {
            colliders.push_back(*i);
        }
    }
    return colliders;
}

void physics_body::update_pos_() {
}

void physics_body::enter_() {
    physics_body::physics_bodies_.push_back(this);
}

void physics_body::exit_() {
    physics_body::physics_bodies_.remove(this);
}

void physics_body::tick_() {
}
