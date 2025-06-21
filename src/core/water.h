#pragma once
#include "engine/physics-body.h"
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

class water : public physics_body {
public:
#ifdef DEV
    std::string debug_name{"water"};
#endif

    float water_level{0.0f};

    water();

protected:
    void body_entered_(physics_body* body) override;
    void area_entered_(physics_body* area) override;
    void enter_() override;
    void exit_() override;
};
