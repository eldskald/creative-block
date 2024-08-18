#pragma once
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include <raylib.h>

class falling_leaves : public game_element {
public:
    falling_leaves();

protected:
    void tick_() override;

private:
    class particle_ : public physics_body {
    public:
        particle_(Vector2 init_pos, falling_leaves* emitter);

    protected:
        void tick_() override;

    private:
        float time_{0.0f};
        falling_leaves* emitter_{nullptr};
    };

    float to_next_particle_{0.0f};
    bool free_{true};

    void spawn_particle_();
};
