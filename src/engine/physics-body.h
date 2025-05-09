#pragma once
#include "engine/game-element.h"
#include <bitset>
#include <list>
#include <raylib.h>

using namespace std;

class game;
#ifdef DEV
class renderer;
#endif

const int COLLISION_LAYERS = 8;

class physics_body : public game_element {
    friend class game;
#ifdef DEV
    friend class renderer;
#endif

public:
    enum body_type { kinematic, fixed, area };

    Vector2 vel{(Vector2){0}};
    Rectangle collision_box{(Rectangle){0}};
    body_type type{kinematic};
    bool one_way{false};
    bitset<COLLISION_LAYERS> collision_layer{0b00000000};
    bitset<COLLISION_LAYERS> collision_mask{0b00000000};
    bitset<COLLISION_LAYERS> v_collision_mask{0b00000000};

    list<physics_body*> get_bodies_touching_top();
    list<physics_body*> get_bodies_touching_left();
    list<physics_body*> get_bodies_touching_bottom();
    list<physics_body*> get_bodies_touching_right();
    list<physics_body*> get_detected_bodies();
    bool collision_detected();

    static list<physics_body*> get_colliders(Rectangle collision_box,
                                             bitset<COLLISION_LAYERS> mask,
                                             bitset<COLLISION_LAYERS> layer);

protected:
    void physics_tick_();

    virtual void body_entered_(physics_body* body);
    virtual void body_exited_(physics_body* body);

    void enter_() override;
    void exit_() override;

private:
    list<physics_body*> detected_bodies_;

    Rectangle get_collision_rect_();
    list<physics_body*> get_physics_body_children_();
    list<Rectangle> get_children_collision_rects_();
    float compute_h_movement_(float delta_d, bool ignore_children = false);
    float compute_v_movement_(float delta_d, bool ignore_children = false);
    float move_and_drag_children_h_(float delta_d);
    float move_and_drag_children_v_(float delta_d);
    float move_and_drag_children_up_(float delta_d);
    float move_and_drag_children_down_(float delta_d);
    list<physics_body*> find_entering_bodies_();
    list<physics_body*> find_exiting_bodies_();

    static list<physics_body*> bodies_;
    static list<physics_body*> areas_;

    static void trigger_physics_tick_(game_element* element);
    static void update_areas_();
};
