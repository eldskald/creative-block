#pragma once
#include "engine/physics-body.h"
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

class killbox : public physics_body {
public:
#ifdef DEV
    std::string debug_name{"killbox"};
#endif

    killbox();

protected:
    void body_entered_(physics_body* body) override;
};
