#pragma once
#include "engine/sprite.h"
#include "core/base-unit.h"
#include "core/death-particles.h"
#include "core/respawn-particles.h"

class player : public base_unit {
public:
    player();

    void emit_respawn_particles();

    void kill() override;

protected:
    void tick_() override;

private:
    float time_{0.0f};
    bool killed_{false};
    death_particles* death_particles_emitter_{nullptr};
    respawn_particles* respawn_particles_emitter_{nullptr};
    sprite* sprite_{nullptr};
};
