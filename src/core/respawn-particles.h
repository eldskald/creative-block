#pragma once
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

class respawn_particles : public game_element {
public:
#ifdef DEV
    std::string debug_name{"respawn_particles"};
#endif

    Color tint{WHITE};

    void emit();

private:
    class particle_ : public physics_body {
    public:
#ifdef DEV
        std::string debug_name{"respawn_particles_particle_"};
#endif

        particle_(Vector2 pos, Vector2 vel, Color tint);

    protected:
        void tick_() override;

    private:
        Vector2 prev_vel_{(Vector2){0}};
        int bounces_{0};
    };
};
