#pragma once
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

class raindrop : public game_element {
public:
#ifdef DEV
    std::string debug_name{"raindrop"};
#endif

    raindrop();

protected:
    void tick_() override;

private:
    class particle_ : public physics_body {
    public:
#ifdef DEV
        std::string debug_name{"raindrop_particle_"};
#endif

        particle_(Vector2 pos);

    protected:
        void tick_() override;

    private:
        void spawn_sub_particles_();
    };

    class sub_particle_ : public physics_body {
    public:
#ifdef DEV
        std::string debug_name{"raindrop_sub_particle_"};
#endif

        sub_particle_(Vector2 pos, Vector2 vel);

    protected:
        void tick_() override;
    };

    float to_next_particle_{0.0f};
    void spawn_particle_();
};
