#pragma once
#include "game-element.h"
#include <raylib.h>

class sfx : public game_element {
public:
    sfx(Sound sound);

    void play();
    void stop();

    static Sound sfx_1;

protected:
    void update_pos_() override;

private:
    Sound sound_;
};
