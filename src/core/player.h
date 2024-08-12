#pragma once
#include "core/base-unit.h"

class player : public base_unit {
public:
    player();

protected:
    void tick_() override;

private:
    float time_{0.0f};
};
