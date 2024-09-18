#pragma once
#include "engine/sprite.h"
#include "core/base-unit.h"
#include "core/death-particles.h"
#include "core/respawn-particles.h"
#include <raylib.h>

class shadow : public base_unit {
public:
    shadow(input_history history);

    void emit_spawn_particles();

    void kill() override;

protected:
    void tick_() override;

private:
    float time_{0.0f};
    Vector2 curr_dir_{(Vector2){0}};
    bool killed_{false};
    bool inputs_ended_{false};
    death_particles* death_particles_emitter_{nullptr};
    respawn_particles* spawn_particles_emitter_{nullptr};
    sprite* sprite_{nullptr};
    input_history history_;

    void read_input_(input input);
};
