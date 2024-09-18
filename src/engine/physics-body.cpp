#include "physics-body.h"
#include "game-element.h"
#include <bitset>
#include <cmath>
#include <list>
#include <raylib.h>
#include <raymath.h>

using namespace std;

list<physics_body*> physics_body::bodies_;
list<physics_body*> physics_body::areas_;

list<physics_body*> physics_body::get_bodies_touching_top() {
    Rectangle rect = (Rectangle){
        floor(this->get_global_pos().x + this->collision_box.x),
        floor(this->get_global_pos().y + this->collision_box.y - 1.0f),
        floor(this->collision_box.width),
        1.0f};
    return physics_body::get_colliders(rect,
                                       this->collision_mask |
                                           this->v_collision_mask,
                                       this->collision_layer);
}

list<physics_body*> physics_body::get_bodies_touching_left() {
    Rectangle rect = (Rectangle){
        floor(this->get_global_pos().x + this->collision_box.x - 1.0f),
        floor(this->get_global_pos().y + this->collision_box.y),
        1.0f,
        floor(this->collision_box.height)};
    return physics_body::get_colliders(
        rect, this->collision_mask, this->collision_layer);
}

list<physics_body*> physics_body::get_bodies_touching_bottom() {
    Rectangle rect =
        (Rectangle){floor(this->get_global_pos().x + this->collision_box.x),
                    floor(this->get_global_pos().y + this->collision_box.y +
                          this->collision_box.height),
                    floor(this->collision_box.width),
                    1.0f};
    return physics_body::get_colliders(rect,
                                       this->collision_mask |
                                           this->v_collision_mask,
                                       this->collision_layer);
}

list<physics_body*> physics_body::get_bodies_touching_right() {
    Rectangle rect =
        (Rectangle){floor(this->get_global_pos().x + this->collision_box.x +
                          this->collision_box.width),
                    floor(this->get_global_pos().y + this->collision_box.y),
                    1.0f,
                    floor(this->collision_box.height)};
    return physics_body::get_colliders(
        rect, this->collision_mask, this->collision_layer);
}

list<physics_body*> physics_body::get_detected_bodies() {
    list<physics_body*> clone;
    for (auto body : this->detected_bodies_) {
        clone.push_back(body);
    }
    return clone;
}

bool physics_body::collision_detected() {
    if (!this->get_bodies_touching_top().empty()) return true;
    if (!this->get_bodies_touching_left().empty()) return true;
    if (!this->get_bodies_touching_bottom().empty()) return true;
    if (!this->get_bodies_touching_right().empty()) return true;
    return false;
}

list<physics_body*>
physics_body::get_colliders(Rectangle collision_box,
                            bitset<COLLISION_LAYERS> mask,
                            bitset<COLLISION_LAYERS> layer) {
    list<physics_body*> colliders;
    for (auto body : physics_body::bodies_) {
        bitset<COLLISION_LAYERS> mask_check = mask;
        mask_check &= body->collision_layer;
        bitset<COLLISION_LAYERS> layer_check = layer;
        layer_check &= body->collision_mask;
        if (CheckCollisionRecs(collision_box, body->get_collision_rect_()) &&
            (mask_check != 0b00000000 || layer_check != 0b00000000)) {
            colliders.push_back(body);
        }
    }
    return colliders;
}

// This function is to get the position in space of the collision shape of any
// given body. We take the floor of everything because colliding with floats is
// inconsistent, so we make every physics rect only move in integer increments.
Rectangle physics_body::get_collision_rect_() {
    return (Rectangle){floor(this->get_global_pos().x + this->collision_box.x),
                       floor(this->get_global_pos().y + this->collision_box.y),
                       floor(this->collision_box.width),
                       floor(this->collision_box.height)};
}

// Calculate where the body would stop if it tried to move delta_d in the X
// axis. Will be equal to delta_d if there are no collisions, or some smaller
// number in case it collides. It is done by getting the rectangle that would
// be the trail of pixels covered by the body as it moved delta_d and finding
// all bodies colliding with it. Bodies already colliding will be ignored.
float physics_body::compute_h_movement_(float delta_d, bool ignore_children) {
    float delta = delta_d;
    if (delta == 0.0f) return 0.0f;

    // The trail. We only take the floor on the Y axis here because we want to
    // be able to move on a float amount, but if we don't take the floor on the
    // other axis, we might detect collisions with bodies that are touching the
    // bottom of this one if it has some fractionary rest because the other one
    // will lose its fraction and go up a little on the calculation.
    Rectangle target_rec =
        (Rectangle){this->get_global_pos().x + this->collision_box.x,
                    floor(this->get_global_pos().y + this->collision_box.y),
                    abs(delta),
                    floor(this->collision_box.height)};
    if (delta > 0.0f)
        target_rec.x += this->collision_box.width;
    else
        target_rec.x += delta;

    // Iterate through all bodies found on the trail.
    list<physics_body*> colliders = physics_body::get_colliders(
        target_rec, this->collision_mask, this->collision_layer);
    for (auto collider : colliders) {

        // Ignore the body if it is among the following:
        if (collider == this) continue;
        if (collider->descends_from(this) && ignore_children) continue;
        if (CheckCollisionRecs(this->get_collision_rect_(),
                               collider->get_collision_rect_()))
            continue;
        if (collider->one_way) continue;

        // Find the final delta to be returned.
        if (delta < 0.0f) {
            float body_left =
                floor(this->get_global_pos().x + this->collision_box.x);
            float coll_right =
                floor(collider->get_global_pos().x + collider->collision_box.x +
                      collider->collision_box.width);
            delta = max(coll_right - body_left, delta);
        } else {
            float body_right =
                floor(this->get_global_pos().x + this->collision_box.x +
                      this->collision_box.width);
            float coll_left =
                floor(collider->get_global_pos().x + collider->collision_box.x);
            delta = min(coll_left - body_right, delta);
        }
    }

    return delta;
}

// The same thing as compute_h_movement_ except for vertical movements. This
// time we have to take one way colliders into account, we want them to
// collide only if the body is coming from above and is not colliding already.
float physics_body::compute_v_movement_(float delta_d, bool ignore_children) {
    float delta = delta_d;
    if (delta == 0.0f) return 0.0f;

    // The trail. We only take the floor on the X axis here because we want to
    // be able to move on a float amount, but if we don't take the floor on the
    // other axis, we might detect collisions with bodies that are touching the
    // right side of this one if it has some fractionary rest because the other
    // one will lose its fraction and go left a little on the calculation.
    Rectangle target_rec =
        (Rectangle){floor(this->get_global_pos().x + this->collision_box.x),
                    this->get_global_pos().y + this->collision_box.y,
                    floor(this->collision_box.width),
                    abs(delta)};
    if (delta > 0.0f)
        target_rec.y += this->collision_box.height;
    else
        target_rec.y += delta;

    // Iterate through all bodies found on the trail.
    list<physics_body*> colliders = physics_body::get_colliders(
        target_rec,
        this->collision_mask | this->v_collision_mask,
        this->collision_layer);
    for (auto collider : colliders) {

        // Ignore the body if it is among the following:
        if (collider == this) continue;
        if (collider->descends_from(this) && ignore_children) continue;
        if (CheckCollisionRecs(this->get_collision_rect_(),
                               collider->get_collision_rect_()))
            continue;
        if (collider->one_way && delta < 0.0f) continue;

        // Find the final delta to be returned.
        if (delta < 0.0f) {
            float body_top =
                floor(this->get_global_pos().y + this->collision_box.y);
            float coll_bottom =
                floor(collider->get_global_pos().y + collider->collision_box.y +
                      collider->collision_box.height);
            delta = max(coll_bottom - body_top, delta);
        } else {
            float body_bottom =
                floor(this->get_global_pos().y + this->collision_box.y +
                      this->collision_box.height);
            float coll_top =
                floor(collider->get_global_pos().y + collider->collision_box.y);
            delta = min(coll_top - body_bottom, delta);
        }
    }

    return delta;
}

float physics_body::move_and_drag_children_h_(float delta_d) {
    if (delta_d == 0.0f) return 0.0f;
    float delta = this->compute_h_movement_(delta_d, true);

    // Order children by their collision box's global position.
    // The logic is: the children will all move on the same direction, so we
    // should first move the ones in front, and then the second most in front,
    // etc etc. This way we stop bodies that are touching to get separated
    // by a gap equal to the delta.
    list<physics_body*> ordered_children;
    list<game_element*> children = this->get_children();
    for (auto i : children) {
        auto* child = dynamic_cast<physics_body*>(i);
        if (!child) continue;
        if (child->type != kinematic) continue;
        if (ordered_children.empty())
            ordered_children.push_back(child);
        else {
            float child_col_x =
                child->get_global_pos().x + child->collision_box.x;
            float top_col_x = ordered_children.back()->get_global_pos().x +
                              ordered_children.back()->collision_box.x;
            if (child_col_x >= top_col_x)
                ordered_children.push_back(child);
            else {
                if (delta >= 0.0f) {
                    for (auto j = ordered_children.begin();
                         j != ordered_children.end();
                         ++j) {
                        float body_col_x =
                            (*j)->get_global_pos().x + (*j)->collision_box.x;
                        if (child_col_x >= body_col_x) {
                            ordered_children.emplace(j, child);
                            break;
                        }
                    }
                } else {
                    for (auto j = ordered_children.begin();
                         j != ordered_children.end();
                         ++j) {
                        float body_col_x =
                            (*j)->get_global_pos().x + (*j)->collision_box.x;
                        if (child_col_x < body_col_x) {
                            ordered_children.emplace(j, child);
                            break;
                        }
                    }
                }
            }
        }
    }

    // Call this function on all this body's children, now that they are
    // ordered.
    for (auto i : ordered_children) {
        i->move_and_drag_children_h_(delta);
    }

    // After we moved all its children, we calculate the movement again, this
    // time allowing it to collide with its children and then move it.
    delta = this->compute_h_movement_(delta);
    this->pos.x += delta;

    // Remember we already moved all the children, but changing this body's
    // position will also change the children's global position, so we have to
    // correct it.
    for (auto i : ordered_children) {
        i->pos.x -= delta;
    }

    return delta;
}

float physics_body::move_and_drag_children_v_(float delta_d) {
    if (delta_d == 0.0f) return 0.0f;
    float delta = this->compute_v_movement_(delta_d, true);

    // Order children by their collision box's global position.
    // The logic is: the children will all move on the same direction, so we
    // should first move the ones in front, and then the second most in front,
    // etc etc. This way we stop bodies that are touching to get separated
    // by a gap equal to the delta.
    list<physics_body*> ordered_children;
    list<game_element*> children = this->get_children();
    for (auto i : children) {
        auto* child = dynamic_cast<physics_body*>(i);
        if (!child) continue;
        if (child->type != kinematic) continue;
        if (ordered_children.empty())
            ordered_children.push_back(child);
        else {
            float child_col_y =
                child->get_global_pos().y + child->collision_box.y;
            float top_col_y = ordered_children.back()->get_global_pos().y +
                              ordered_children.back()->collision_box.y;
            if (child_col_y >= top_col_y)
                ordered_children.push_back(child);
            else {
                if (delta >= 0.0f) {
                    for (auto j = ordered_children.begin();
                         j != ordered_children.end();
                         ++j) {
                        float body_col_y =
                            (*j)->get_global_pos().y + (*j)->collision_box.y;
                        if (child_col_y >= body_col_y) {
                            ordered_children.emplace(j, child);
                            break;
                        }
                    }
                } else {
                    for (auto j = ordered_children.begin();
                         j != ordered_children.end();
                         ++j) {
                        float body_col_y =
                            (*j)->get_global_pos().y + (*j)->collision_box.y;
                        if (child_col_y < body_col_y) {
                            ordered_children.emplace(j, child);
                            break;
                        }
                    }
                }
            }
        }
    }

    // Call this function on all this body's children, now that they are
    // ordered.
    for (auto i : ordered_children) {
        i->move_and_drag_children_v_(delta);
    }

    // After we moved all its children, we calculate the movement again, this
    // time allowing it to collide with its children and then move it.
    delta = this->compute_v_movement_(delta);
    this->pos.y += delta;

    // Remember we already moved all the children, but changing this body's
    // position will also change the children's global position, so we have to
    // correct it.
    for (auto i : ordered_children) {
        i->pos.y -= delta;
    }

    return delta;
}

void physics_body::physics_tick_() {
    float delta_x = this->vel.x * GetFrameTime();
    float delta_y = this->vel.y * GetFrameTime();
    if (this->type == kinematic) {
        if (!FloatEquals(delta_x, this->move_and_drag_children_h_(delta_x))) {
            this->vel.x = 0.0f;
        }
        if (!FloatEquals(delta_y, this->move_and_drag_children_v_(delta_y))) {
            this->vel.y = 0.0f;
        }
    } else {
        this->pos.x += delta_x;
        this->pos.y += delta_y;
    }
}

// This is for the game class to call on the root scene once and trigger
// all physics ticks in order of parent first.
void physics_body::trigger_physics_tick_(game_element* element) {
    if (!element) return;
    auto body = dynamic_cast<physics_body*>(element);
    if (body) body->physics_tick_();
    list<game_element*> children = element->get_children();
    for (auto i : children) {
        auto child = dynamic_cast<physics_body*>(i);
        if (child) physics_body::trigger_physics_tick_(child);
    }
}

void physics_body::enter_() {
    switch (this->type) {
    case kinematic:
        physics_body::bodies_.push_back(this);
        break;
    case fixed:
        physics_body::bodies_.push_back(this);
        break;
    case area:
        physics_body::areas_.push_back(this);
        break;
    }
}

void physics_body::exit_() {
    switch (this->type) {
    case kinematic:
        physics_body::bodies_.remove(this);
        break;
    case fixed:
        physics_body::bodies_.remove(this);
        break;
    case area:
        physics_body::areas_.remove(this);
        break;
    }
}

list<physics_body*> physics_body::find_entering_bodies_() {
    list<physics_body*> found_bodies;

    list<physics_body*> colliders =
        physics_body::get_colliders(this->get_collision_rect_(),
                                    this->collision_mask,
                                    this->collision_layer);
    for (auto collider : colliders) {
        bool found = true;
        for (auto body : this->detected_bodies_) {
            if (body == collider) {
                found = false;
                break;
            }
        }
        if (found) found_bodies.push_back(collider);
    }

    for (auto body : found_bodies) {
        this->detected_bodies_.push_back(body);
    }

    return found_bodies;
}

list<physics_body*> physics_body::find_exiting_bodies_() {
    list<physics_body*> found_bodies;
    list<list<physics_body*>::iterator> iterators;

    list<physics_body*> colliders =
        physics_body::get_colliders(this->get_collision_rect_(),
                                    this->collision_mask,
                                    this->collision_layer);
    for (auto i = this->detected_bodies_.begin();
         i != this->detected_bodies_.end();
         ++i) {
        bool found = true;
        for (auto collider : colliders) {
            if (collider == *i) {
                found = false;
                break;
            }
        }
        if (found) {
            found_bodies.push_back(*i);
            iterators.push_back(i);
        }
    }

    for (auto iterator : iterators) {
        this->detected_bodies_.erase(iterator);
    }

    return found_bodies;
}

void physics_body::update_areas_() {
    for (auto area : physics_body::areas_) {
        list<physics_body*> entering_bodies = area->find_entering_bodies_();
        list<physics_body*> exiting_bodies = area->find_exiting_bodies_();

        for (auto body : entering_bodies) {
            area->body_entered_(body);
        }
        for (auto body : exiting_bodies) {
            area->body_exited_(body);
        }
    }
}

void physics_body::body_entered_(physics_body* body) {
}

void physics_body::body_exited_(physics_body* body) {
}
