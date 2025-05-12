#include "core/goal.h"
#include "core/base-unit.h"
#include "core/scene-manager.h"
#include "engine/physics-body.h"
#include "engine/sfx.h"

goal::goal() {
    this->type = physics_body::area;
}

void goal::body_entered_(physics_body* body) {
    auto* unit_element = dynamic_cast<base_unit*>(body);
    if (unit_element) {
        sfx::play(sfx::next_level);
        scene_manager::next_scene();
    }
}
