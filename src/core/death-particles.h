#pragma once
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include <raylib.h>

class death_particles : public game_element {
public:
    Color tint{WHITE};

    void emit();

private:
    class particle_ : public physics_body {
    public:
        particle_(Vector2 pos, Vector2 vel, Color tint);

    protected:
        void tick_() override;

    private:
        Vector2 prev_vel_;
        int bounces_{0};
    };
};
