#pragma once
#include "engine/game-element.h"
#include "engine/text.h"

#include <raylib.h>

class credits : public game_element {
public:
    credits();

protected:
    void tick_() override;

private:
    text* message_{nullptr};
    float time_{0.0f};
};
