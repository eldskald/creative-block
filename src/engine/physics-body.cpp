#include "engine/physics-body.h"
#include "engine/game-element.h"
#include <bitset>
#include <cmath>
#include <list>
#include <raylib.h>
#include <raymath.h>

using namespace std;

list<physics_body*> physics_body::bodies_;
list<physics_body*> physics_body::areas_;

// This function is to get the position in space of the collision shape of any
// given body. We take the floor of everything because colliding with floats is
// inconsistent, so we make every physics rect only move in integer increments.
Rectangle physics_body::get_collision_rect_() {
    return (Rectangle){floor(this->get_global_pos().x + this->collision_box.x),
                       floor(this->get_global_pos().y + this->collision_box.y),
                       floor(this->collision_box.width),
                       floor(this->collision_box.height)};
}

list<physics_body*> physics_body::get_overlapping_bodies() {
    list<physics_body*> bodies =
        physics_body::get_colliders(this->get_collision_rect_(),
                                    this->collision_mask,
                                    this->collision_layer);
    bodies.remove(this);
    return bodies;
}

list<physics_body*> physics_body::get_bodies_touching_top() {
    Rectangle rect = (Rectangle){
        floor(this->get_global_pos().x + this->collision_box.x),
        floor(this->get_global_pos().y + this->collision_box.y - 1.0f),
        floor(this->collision_box.width),
        1.0f};
    auto bodies = physics_body::get_colliders(
        rect, this->collision_mask, this->collision_layer);

    // Only count bodies that aren't overlapping
    list<physics_body*> touching_bodies;
    for (auto* body : bodies) {
        if (!CheckCollisionRecs(body->get_collision_rect_(),
                                this->get_collision_rect_())) {
            touching_bodies.push_back(body);
        }
    }
    return touching_bodies;
}

list<physics_body*> physics_body::get_bodies_touching_left() {
    Rectangle rect = (Rectangle){
        floor(this->get_global_pos().x + this->collision_box.x - 1.0f),
        floor(this->get_global_pos().y + this->collision_box.y),
        1.0f,
        floor(this->collision_box.height)};
    auto bodies = physics_body::get_colliders(
        rect, this->collision_mask, this->collision_layer);

    // Only count bodies that aren't overlapping
    list<physics_body*> touching_bodies;
    for (auto* body : bodies) {
        if (!CheckCollisionRecs(body->get_collision_rect_(),
                                this->get_collision_rect_())) {
            touching_bodies.push_back(body);
        }
    }
    return touching_bodies;
}

list<physics_body*> physics_body::get_bodies_touching_bottom() {
    Rectangle rect =
        (Rectangle){floor(this->get_global_pos().x + this->collision_box.x),
                    floor(this->get_global_pos().y + this->collision_box.y +
                          this->collision_box.height),
                    floor(this->collision_box.width),
                    1.0f};
    auto bodies = physics_body::get_colliders(
        rect, this->collision_mask, this->collision_layer);

    // Only count bodies that aren't overlapping
    list<physics_body*> touching_bodies;
    for (auto* body : bodies) {
        if (!CheckCollisionRecs(body->get_collision_rect_(),
                                this->get_collision_rect_())) {
            touching_bodies.push_back(body);
        }
    }
    return touching_bodies;
}

list<physics_body*> physics_body::get_bodies_touching_right() {
    Rectangle rect =
        (Rectangle){floor(this->get_global_pos().x + this->collision_box.x +
                          this->collision_box.width),
                    floor(this->get_global_pos().y + this->collision_box.y),
                    1.0f,
                    floor(this->collision_box.height)};
    auto bodies = physics_body::get_colliders(
        rect, this->collision_mask, this->collision_layer);

    // Only count bodies that aren't overlapping
    list<physics_body*> touching_bodies;
    for (auto* body : bodies) {
        if (!CheckCollisionRecs(body->get_collision_rect_(),
                                this->get_collision_rect_())) {
            touching_bodies.push_back(body);
        }
    }
    return touching_bodies;
}

list<physics_body*> physics_body::get_detected_bodies() {
    list<physics_body*> clone;
    for (auto body : this->detected_bodies_) {
        clone.push_back(body);
    }
    return clone;
}

list<physics_body*> physics_body::get_detected_areas() {
    list<physics_body*> clone;
    for (auto body : this->detected_areas_) {
        clone.push_back(body);
    }
    return clone;
}

physics_body* physics_body::get_carrier() {
    return this->carried_by_;
}

list<physics_body*> physics_body::get_carried_bodies() {
    list<physics_body*> clone;
    for (auto* body : this->carried_bodies_) {
        clone.push_back(body);
    }
    return clone;
}

void physics_body::carry(physics_body* body) {
    if (body->carried_by_ == this) return;
    if (body->carried_by_) body->carried_by_->let_go_of(body);
    this->carried_bodies_.push_back(body);
    body->carried_by_ = this;
}

void physics_body::let_go_of(physics_body* body) {
    if (body->carried_by_ != this) return;
    this->carried_bodies_.remove(body);
    body->carried_by_ = nullptr;
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

list<physics_body*>
physics_body::get_intersecting_areas(Rectangle collision_box,
                                     bitset<COLLISION_LAYERS> mask,
                                     bitset<COLLISION_LAYERS> layer) {
    list<physics_body*> colliders;
    for (auto body : physics_body::areas_) {
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

// Calculate where the body would stop if it tried to move delta_d in the X
// axis. Will be equal to delta_d if there are no collisions, or some smaller
// number in case it collides. It is done by getting the rectangle that would
// be the trail of pixels covered by the body as it moved delta_d and finding
// all bodies colliding with it. Bodies already colliding will be ignored.
float physics_body::compute_h_movement_(float delta_d, bool ignore_children) {
    float delta = delta_d;

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
        target_rec, this->collision_mask, this->collision_layer);
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

// Move the body and then bring the carrieds on top back the same distance that
// was moved, because as we move the body we will automatically move its
// children without taking physics into account, so we need to move them back
// and call this same function on each one of them. We don't need to worry about
// collisions with the carrieds because they are all on top of the body and
// can't collide with it. We also don't need to worry about collisions between
// the carreids because they can't collide on the X axis, only on the Y.
float physics_body::move_and_drag_carrieds_h_(float delta_d) {
    if (FloatEquals(delta_d, 0.0f)) return 0.0f;
    float delta = this->compute_h_movement_(delta_d, true);
    if (FloatEquals(delta, 0.0f)) return 0.0f;

    this->pos.x += delta;
    for (auto i : this->carried_bodies_) {
        i->move_and_drag_carrieds_h_(delta);
    }

    return delta;
}

float physics_body::move_and_drag_carrieds_v_(float delta_d) {
    if (FloatEquals(delta_d, 0.0f)) return 0.0f;
    if (delta_d > 0.0f) return this->move_and_drag_carrieds_down_(delta_d);
    return this->move_and_drag_carrieds_up_(delta_d);
}

// When going up, we should move the carrieds from top to bottom. While moving
// them, we reuse the delta because this guarantees the delta will only go down
// as they collide, so we don't move the following bodies more than necessary.
float physics_body::move_and_drag_carrieds_up_(float delta_d) {
    float delta = delta_d;
    for (auto i : this->carried_bodies_) {
        delta = i->move_and_drag_carrieds_up_(delta);
    }
    delta = this->compute_v_movement_(delta);
    this->pos.y += delta;
    return delta;
}

// When going down, move them from bottom to top.
float physics_body::move_and_drag_carrieds_down_(float delta_d) {
    float delta = this->compute_v_movement_(delta_d);
    this->pos.y += delta;
    for (auto i : this->carried_bodies_) {
        i->move_and_drag_carrieds_down_(delta);
    }
    return delta;
}

void physics_body::physics_tick_() {
    float delta_x = this->vel.x * GetFrameTime();
    float delta_y = this->vel.y * GetFrameTime();
    if (this->type == kinematic) {
        if (!FloatEquals(delta_x, this->move_and_drag_carrieds_h_(delta_x))) {
            this->vel.x = 0.0f;
        }
        if (!FloatEquals(delta_y, this->move_and_drag_carrieds_v_(delta_y))) {
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

list<physics_body*> physics_body::find_entering_areas_() {
    list<physics_body*> found_areas;

    list<physics_body*> intersectors =
        physics_body::get_intersecting_areas(this->get_collision_rect_(),
                                             this->collision_mask,
                                             this->collision_layer);
    for (auto intersector : intersectors) {
        if (intersector == this) continue;
        bool found = true;
        for (auto area : this->detected_areas_) {
            if (area == intersector) {
                found = false;
                break;
            }
        }
        if (found) found_areas.push_back(intersector);
    }

    for (auto area : found_areas) {
        this->detected_areas_.push_back(area);
    }

    return found_areas;
}

list<physics_body*> physics_body::find_exiting_areas_() {
    list<physics_body*> found_areas;
    list<list<physics_body*>::iterator> iterators;

    list<physics_body*> intersectors =
        physics_body::get_intersecting_areas(this->get_collision_rect_(),
                                             this->collision_mask,
                                             this->collision_layer);
    for (auto i = this->detected_areas_.begin();
         i != this->detected_areas_.end();
         ++i) {
        bool found = true;
        for (auto intersector : intersectors) {
            if (intersector == *i) {
                found = false;
                break;
            }
        }
        if (found) {
            found_areas.push_back(*i);
            iterators.push_back(i);
        }
    }

    for (auto iterator : iterators) {
        this->detected_areas_.erase(iterator);
    }

    return found_areas;
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

        list<physics_body*> entering_areas = area->find_entering_areas_();
        list<physics_body*> exiting_areas = area->find_exiting_areas_();

        for (auto body : entering_areas) {
            area->area_entered_(body);
        }
        for (auto body : exiting_areas) {
            area->area_exited_(body);
        }
    }
}

void physics_body::body_entered_(physics_body* body) {
}

void physics_body::body_exited_(physics_body* body) {
}

void physics_body::area_entered_(physics_body* area) {
}

void physics_body::area_exited_(physics_body* area) {
}
