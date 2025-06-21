#pragma once
#include "engine/game-element.h"
#include "engine/text.h"
#include <raylib.h>
#ifdef DEV
#include <string>
#endif

class credits : public game_element {
public:
#ifdef DEV
    std::string debug_name{"credits"};
#endif

    credits();

protected:
    void tick_() override;

private:
    text* message_{nullptr};
    float time_{0.0f};
};
