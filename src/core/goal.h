#pragma once
#include "engine/physics-body.h"
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

class goal : public physics_body {
public:
#ifdef DEV
    std::string debug_name{"goal"};
#endif

    goal();

protected:
    void body_entered_(physics_body* body) override;
};
