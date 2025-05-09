#pragma once

#include "engine/physics-body.h"
#include <raylib.h>

class killbox : public physics_body {
public:
    killbox();

protected:
    void body_entered_(physics_body* body) override;
};
