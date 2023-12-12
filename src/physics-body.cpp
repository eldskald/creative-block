#include "physics-body.h"
#include "game-element.h"
#include <cmath>
#include <list>
#include <raylib.h>

using namespace std;

list<physics_body*> physics_body::physics_bodies_;

physics_body::physics_body() {
    this->vel = (Vector2){0};
    this->collision_box = (Rectangle){0};
    this->static_body = false;
}

list<physics_body*> physics_body::get_colliders(Rectangle collision_box) {
    list<physics_body*> colliders;
    for (auto i = physics_body::physics_bodies_.begin();
         i != physics_body::physics_bodies_.end();
         ++i) {
        Rectangle worldSpaceRec =
            (Rectangle){(*i)->collision_box.x + (*i)->global_pos_.x,
                        (*i)->collision_box.y + (*i)->global_pos_.y,
                        (*i)->collision_box.width,
                        (*i)->collision_box.height};
        if (CheckCollisionRecs(collision_box, worldSpaceRec)) {
            colliders.push_back(*i);
        }
    }
    return colliders;
}

void physics_body::update_pos_() {
    if (!this->static_body) {
        Vector2 delta_d = (Vector2){this->vel.x * GetFrameTime(),
                                    this->vel.y * GetFrameTime()};

        // Move it on the X axis first. It checks the whole X axis trail the
        // body is attempting to cover, check for collisions and updates
        // delta_d.x to be just enough to touch but not collide.
        Rectangle target_rec = (Rectangle){
            this->collision_box.x + this->global_pos_.x + min(0.0f, delta_d.x),
            this->collision_box.y + this->global_pos_.y,
            this->collision_box.width + abs(delta_d.x),
            this->collision_box.height};
        list<physics_body*> colliders = physics_body::get_colliders(target_rec);
        for (auto i = colliders.begin(); i != colliders.end(); ++i) {
            if (*i == this) continue;
            if (vel.x < 0) {
                float body_side = this->global_pos_.x + this->collision_box.x;
                float coll_side = (*i)->global_pos_.x + (*i)->collision_box.x +
                                  (*i)->collision_box.width;
                delta_d.x = max(coll_side - body_side, delta_d.x);
            } else {
                float body_side = this->global_pos_.x + this->collision_box.x +
                                  this->collision_box.width;
                float coll_side = (*i)->global_pos_.x + (*i)->collision_box.x;
                delta_d.x = min(coll_side - body_side, delta_d.x);
            }
        }

        // Move it on the Y axis now, same idea as the X axis except the body
        // has already been moved on the X axis.
        target_rec = (Rectangle){
            this->collision_box.x + this->global_pos_.x + delta_d.x,
            this->collision_box.y + this->global_pos_.y + min(0.0f, delta_d.y),
            this->collision_box.width,
            this->collision_box.height + abs(delta_d.y)};
        colliders = physics_body::get_colliders(target_rec);
        for (auto i = colliders.begin(); i != colliders.end(); ++i) {
            if (*i == this) continue;
            if (vel.y < 0) {
                float body_side = this->global_pos_.y + this->collision_box.y;
                float coll_side = (*i)->global_pos_.y + (*i)->collision_box.y +
                                  (*i)->collision_box.height;
                delta_d.y = max(coll_side - body_side, delta_d.y);
            } else {
                float body_side = this->global_pos_.y + this->collision_box.y +
                                  this->collision_box.height;
                float coll_side = (*i)->global_pos_.y + (*i)->collision_box.y;
                delta_d.y = min(coll_side - body_side, delta_d.y);
            }
        }

        // Move the body and update its velocity.
        this->pos.x += delta_d.x;
        this->pos.y += delta_d.y;
        this->vel.x = delta_d.x / GetFrameTime();
        this->vel.y = delta_d.y / GetFrameTime();
    }
    game_element::update_pos_();
}

void physics_body::enter_() {
    physics_body::physics_bodies_.push_back(this);
}

void physics_body::exit_() {
    physics_body::physics_bodies_.remove(this);
}
