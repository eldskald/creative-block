#pragma once
#include "engine/game-element.h"
#include "engine/text.h"
#include <raylib.h>

class opening : public game_element {
public:
    opening();

protected:
    void tick_() override;

private:
    text* message_{nullptr};
    float time_{0.0f};
};
