#pragma once
#include "engine/sprite.h"
#include "core/base-unit.h"
#include "core/death-particles.h"
#include "core/respawn-particles.h"
#include <raylib.h>
#include <list>

class shadow : public base_unit {
public:
    shadow(input_history history);

    void set_iterator_to_self(list<shadow*>::iterator it);
    list<shadow*>::iterator get_iterator_to_self();
    void emit_spawn_particles();
    void remove();

    void kill() override;

protected:
    void tick_() override;

private:
    float time_{0.0f};
    Vector2 curr_dir_{(Vector2){0}};
    bool killed_{false};
    list<shadow*>::iterator iterator_to_self_{nullptr};
    death_particles* death_particles_emitter_{nullptr};
    respawn_particles* spawn_particles_emitter_{nullptr};
    sprite* sprite_{nullptr};
    input_history history_;

    void read_input_(input input);
};
