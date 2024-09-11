#include "core/killbox.h"
#include "core/base-unit.h"
#include "engine/physics-body.h"

killbox::killbox() {
    this->type = physics_body::area;
}

void killbox::body_entered_(physics_body* body) {
    auto* unit_element = dynamic_cast<base_unit*>(body);
    if (unit_element) {
        unit_element->kill();
    }
}
