#pragma once
#include "engine/inputs.h"
#include "engine/physics-body.h"
#include <list>
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

using input = struct input {
    inputs::action action;
    bool pressed;
    float time;
};

using input_history = list<input>;

class base_unit : public physics_body {
public:
#ifdef DEV
    std::string debug_name{"base_unit"};
#endif

    void change_dir(Vector2 dir);
    void press_jump();
    void release_jump();

    Vector2 get_dir();
    bool is_grounded();

    virtual void kill();

protected:
    void tick_() override;
    void marked_for_deletion_() override;

    virtual void jumped_();

private:
    Vector2 dir_{(Vector2){0}};
    float jump_buffer_timer_{0.0f};
    bool is_pre_buffering_jump_{false};
    bool is_jumping_{false};
};
