#pragma once
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include <raylib.h>

class water_drip : public sprite {
public:
    water_drip();

protected:
    void tick_() override;

private:
    class particle_ : public physics_body {
    public:
        particle_(Vector2 pos);

    protected:
        void tick_() override;

    private:
        void spawn_sub_particles_();
    };

    class sub_particle_ : public physics_body {
    public:
        sub_particle_(Vector2 pos, Vector2 vel);

    protected:
        void tick_() override;
    };

    float to_next_particle_{0.0f};
    void spawn_particles_();
};
