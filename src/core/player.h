#pragma once
#include "core/base-unit.h"

class player : public base_unit {
public:
    player();

    void kill() override;

protected:
    void tick_() override;

private:
    float time_{0.0f};
    bool killed_{false};
};
