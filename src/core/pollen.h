#pragma once
#include "engine/game-element.h"
#include "engine/physics-body.h"
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

class pollen : public game_element {
public:
#ifdef DEV
    std::string debug_name{"pollen"};
#endif

    pollen();

protected:
    void tick_() override;

private:
    class particle_ : public physics_body {
    public:
#ifdef DEV
        std::string debug_name{"pollen_particle_"};
#endif

        particle_(Vector2 init_pos);

    protected:
        void tick_() override;

    private:
        float time_{0.0f};
        float lifetime_{0.0f};
    };

    float to_next_particle_{0.0f};

    void spawn_particle_();
};
