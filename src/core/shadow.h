#pragma once
#include "core/base-unit.h"
#include "core/death-particles.h"
#include "core/respawn-particles.h"
#include "engine/sprite.h"
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

class shadow : public base_unit {
public:
#ifdef DEV
    std::string debug_name{"shadow"};
#endif

    shadow(input_history history);

    void emit_spawn_particles();

    void kill() override;

protected:
    void tick_() override;
    void on_player_death_() override;

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
