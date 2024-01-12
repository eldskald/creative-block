#include "physics-body.h"
#include "game-element.h"
#include <cmath>
#include <list>
#include <raylib.h>
#include <raymath.h>

using namespace std;

list<physics_body*> physics_body::physics_bodies_;

physics_body::physics_body() {
    this->vel = Vector2Zero();
    this->collision_box = (Rectangle){0};
    this->static_body = false;
    this->one_way = false;
    this->prev_global_pos_ = Vector2Zero();
}

list<physics_body*> physics_body::get_colliders(Rectangle collision_box) {
    list<physics_body*> colliders;
    for (auto i = physics_body::physics_bodies_.begin();
         i != physics_body::physics_bodies_.end();
         ++i) {
        Rectangle worldSpaceRec =
            (Rectangle){(*i)->collision_box.x + (*i)->get_global_pos().x,
                        (*i)->collision_box.y + (*i)->get_global_pos().y,
                        (*i)->collision_box.width,
                        (*i)->collision_box.height};
        if (CheckCollisionRecs(collision_box, worldSpaceRec)) {
            colliders.push_back(*i);
        }
    }
    return colliders;
}

Vector2 physics_body::get_extrusion_from_(Rectangle rect) {
    Rectangle this_box =
        (Rectangle){this->get_global_pos().x + this->collision_box.x,
                    this->get_global_pos().y + this->collision_box.y,
                    this->collision_box.width,
                    this->collision_box.height};
    if (!CheckCollisionRecs(rect, this_box)) return Vector2Zero();

    // The left/right/top/bottom sides of the collision boxes.
    float body_top = rect.y;
    float body_bottom = rect.y + rect.height;
    float body_left = rect.x;
    float body_right = rect.x + rect.width;
    float this_top = this->get_global_pos().y + this->collision_box.y;
    float this_bottom = this->get_global_pos().y + this->collision_box.y +
                        this->collision_box.height;
    float this_left = this->get_global_pos().x + this->collision_box.x;
    float this_right = this->get_global_pos().x + this->collision_box.x +
                       this->collision_box.width;

    // The amount this body would be extruded by or a large number, because we
    // are going to get the smallest value out of these four.
    float move_up = body_top < this_bottom ? this_bottom - body_top : 999.0f;
    float move_down = body_bottom > this_top ? body_bottom - this_top : 999.0f;
    float move_left = body_left < this_right ? this_right - body_left : 999.0f;
    float move_right = body_right > this_left ? body_right - this_left : 999.0f;

    // Assuming it just moved into the body, the collision box should be just
    // a little bit inside in the direction it tried to move, so the smallest
    // of these should be the the amount we have to move this body by.
    float horizontal_delta = 0.0f;
    if (move_left <= move_right)
        horizontal_delta = -move_left;
    else
        horizontal_delta = move_right;
    float vertical_delta = 0.0f;
    if (move_up <= move_down)
        vertical_delta = -move_up;
    else
        vertical_delta = move_down;
    if (abs(horizontal_delta) < abs(vertical_delta)) {
        return (Vector2){horizontal_delta, 0.0f};
    } else {
        return (Vector2){0.0f, vertical_delta};
    }
}

float physics_body::compute_h_movement_(float delta_d) {
    float delta = delta_d;
    if (delta == 0) return 0.0f;

    Rectangle target_rec =
        (Rectangle){this->get_global_pos().x + this->collision_box.x,
                    this->get_global_pos().y + this->collision_box.y,
                    abs(delta),
                    this->collision_box.height};
    if (delta > 0)
        target_rec.x += this->collision_box.width;
    else
        target_rec.x += delta;

    list<physics_body*> colliders = physics_body::get_colliders(target_rec);
    for (auto i = colliders.begin(); i != colliders.end(); ++i) {
        if (*i == this) continue;
        if ((*i)->descends_from(this)) continue;
        if ((*i)->one_way) continue;
        if (delta < 0) {
            float body_side = this->get_global_pos().x + this->collision_box.x;
            float coll_side = (*i)->get_global_pos().x + (*i)->collision_box.x +
                              (*i)->collision_box.width;
            delta = max(coll_side - body_side, delta);
        } else {
            float body_side = this->get_global_pos().x + this->collision_box.x +
                              this->collision_box.width;
            float coll_side = (*i)->get_global_pos().x + (*i)->collision_box.x;
            delta = min(coll_side - body_side, delta);
        }
    }

    return delta;
}

float physics_body::compute_v_movement_(float delta_d) {
    float delta = delta_d;
    if (delta == 0) return 0.0f;

    Rectangle target_rec =
        (Rectangle){this->get_global_pos().x + this->collision_box.x,
                    this->get_global_pos().y + this->collision_box.y,
                    this->collision_box.width,
                    abs(delta)};
    if (delta > 0)
        target_rec.y += this->collision_box.height;
    else
        target_rec.y += delta;

    list<physics_body*> colliders = physics_body::get_colliders(target_rec);
    for (auto i = colliders.begin(); i != colliders.end(); ++i) {
        if (*i == this) continue;
        if ((*i)->descends_from(this)) continue;
        if ((*i)->one_way) {
            Rectangle body_rect =
                (Rectangle){(*i)->get_global_pos().x + (*i)->collision_box.x,
                            (*i)->get_global_pos().y + (*i)->collision_box.y,
                            (*i)->collision_box.width,
                            (*i)->collision_box.height};
            Rectangle this_rect =
                (Rectangle){this->get_global_pos().x + this->collision_box.x,
                            this->get_global_pos().y + this->collision_box.y,
                            this->collision_box.width,
                            this->collision_box.height};
            if (CheckCollisionRecs(this_rect, body_rect)) continue;
            if (delta < 0.0f) continue;
        }
        if (delta < 0) {
            float body_side = this->get_global_pos().y + this->collision_box.y;
            float coll_side = (*i)->get_global_pos().y + (*i)->collision_box.y +
                              (*i)->collision_box.height;
            delta = max(coll_side - body_side, delta);
        } else {
            float body_side = this->get_global_pos().y + this->collision_box.y +
                              this->collision_box.height;
            float coll_side = (*i)->get_global_pos().y + (*i)->collision_box.y;
            delta = min(coll_side - body_side, delta);
        }
    }

    return delta;
}

// This is for the game class to call on the root scene once and trigger
// all physics ticks in order of parent first.
void physics_body::trigger_physics_tick_(game_element* element) {
    physics_body* body = dynamic_cast<physics_body*>(element);
    if (body) body->physics_tick_();
    list<game_element*> children = element->get_children();
    for (auto i = children.begin(); i != children.end(); ++i) {
        physics_body* child = dynamic_cast<physics_body*>(*i);
        if (child) physics_body::trigger_physics_tick_(child);
    }
}

void physics_body::physics_tick_() {
    if (this->static_body) {

        // First, for static bodies we want them to ignore collisions with
        // other static bodies and extrude non static bodies out. We also ignore
        // their velocities.
        if (this->one_way) return;
        for (auto i = physics_body::physics_bodies_.begin();
             i != physics_body::physics_bodies_.end();
             ++i) {
            if ((*i)->static_body) continue;
            Rectangle target_rect =
                (Rectangle){this->get_global_pos().x + this->collision_box.x,
                            this->get_global_pos().y + this->collision_box.y,
                            this->collision_box.width,
                            this->collision_box.height};
            Vector2 delta = (*i)->get_extrusion_from_(target_rect);
            (*i)->pos = Vector2Add((*i)->pos, delta);
        }
    } else {

        // Now, for non-static bodies.

        // All its parents have already been moved and collided, so this one
        // was dragged along but not collided yet. To correct it, we subtract
        // the this->pos from this->get_global_pos to obtain the current
        // parent's global pos, and because we haven't changed pos yet,
        // subtracting this->pos from this->prev_global_pos_ will yield the
        // global pos of the parent before getting moved this frame. With both
        // of these, we get the movement this body got dragged this frame.
        Vector2 parent_global_pos =
            Vector2Subtract(this->get_global_pos(), this->pos);
        Vector2 parent_prev_global_pos =
            Vector2Subtract(this->prev_global_pos_, this->pos);
        Vector2 drag =
            Vector2Subtract(parent_global_pos, parent_prev_global_pos);
        // Now we subtract current drag to get it back to its previous global
        // pos, then we move and collide it to try and put it where it got
        // dragged into. We have to ignore the parent because the only way
        // the drag could cause them to collide is if it collided with
        // something else and the parent collided with it, meaning it should
        // be extruded instead of handled with the compute functions
        // (compute functions stop the 'this' from going inside anything
        // but we're trying to stop the parent from going inside 'this').
        this->pos = Vector2Subtract(this->pos, drag);
        drag.x = this->compute_h_movement_(drag.x);
        drag.y = this->compute_v_movement_(drag.y);
        this->pos = Vector2Add(this->pos, drag);

        // Now, if a collision caused it to stop and collide with a parent that
        // was draggin it along, we should extrude that parent and update their
        // velocity as well. Keep in mind the parent was already moved, and we
        // couldn't have known that a child being dragged along would collide
        // with something which would make it collide with the parent, so only
        // after resolving the child we can move the parent back, hence why we
        // need to extrude it instead of handling with compute functions.
        physics_body* curr_parent =
            dynamic_cast<physics_body*>(this->get_parent());
        while (curr_parent) {
            Rectangle rect =
                (Rectangle){this->get_global_pos().x + this->collision_box.x,
                            this->get_global_pos().y + this->collision_box.y,
                            this->collision_box.width,
                            this->collision_box.height};

            // POTENTIAL BUG: If either the parent or this body are one-way
            // colliders, this is going to treat them as they're not. Do nothing
            // for now as we don't plan on carrying one way colliders, so
            // adding a check would just slow it down. But keep that in the back
            // of your head, right now only the compute functions can correctly
            // deal with one-way colliders, extrusions can't.
            Vector2 diff = curr_parent->get_extrusion_from_(rect);
            if (Vector2Equals(diff, Vector2Zero())) break;
            curr_parent->pos = Vector2Add(curr_parent->pos, diff);

            // After extruding the parent, we need to add the extrusion
            // to this body's pos or else it will get dragged and collide again.
            //
            // POTENTIAL BUG: All of this body's brothers will get dragged by
            // this extrusion, which may cause them to collide with this one.
            // Do nothing for now, as we plan to have all child bodies standing
            // on top of their parent, so extrusions won't cause them to collide
            // because they will all have the same y position and the extrusion
            // should only be vertical due to this. But take this into account
            // in case weird collision bugs happen or you need to generalize
            // this solution.
            this->pos = Vector2Subtract(this->pos, diff);

            // We also need to update the parent's velocity.
            curr_parent->vel =
                Vector2Add(curr_parent->vel, Vector2Scale(diff, GetFPS()));
            curr_parent =
                dynamic_cast<physics_body*>(curr_parent->get_parent());
        }

        // Now finally, we should move the body with its velocity.
        Vector2 delta_d = Vector2Scale(this->vel, GetFrameTime());
        delta_d.x = this->compute_h_movement_(delta_d.x);
        delta_d.y = this->compute_v_movement_(delta_d.y);
        this->pos = Vector2Add(this->pos, delta_d);
        this->vel = Vector2Scale(delta_d, GetFPS());
    }
}

void physics_body::enter_() {
    this->prev_global_pos_ = this->get_global_pos();
    physics_body::physics_bodies_.push_back(this);
}

void physics_body::exit_() {
    this->prev_global_pos_ = Vector2Zero();
    physics_body::physics_bodies_.remove(this);
}

// This is for the game class to call once after the physics tick has finished.
void physics_body::update_all_bodies_prev_global_pos_() {
    for (auto i = physics_body::physics_bodies_.begin();
         i != physics_body::physics_bodies_.end();
         ++i) {
        (*i)->prev_global_pos_ = (*i)->get_global_pos();
    }
}
