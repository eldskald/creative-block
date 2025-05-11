#pragma once
#include "engine/physics-body.h"
#include <raylib.h>

class goal : public physics_body {
public:
    goal();

protected:
    void body_entered_(physics_body* body) override;
};
