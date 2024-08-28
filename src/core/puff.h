#pragma once
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include <raylib.h>

class puff : public game_element {
public:
    puff();

protected:
    void tick_() override;

private:
    class particle_ : public physics_body {
    public:
        particle_(Vector2 init_pos, puff* emitter);

    protected:
        void tick_() override;

    private:
        float time_{0.0f};
        puff* emitter_{nullptr};
        float lifetime_{0.0f};
    };

    float to_next_particle_{0.0f};
    bool free_{true};

    void spawn_particle_();
};
