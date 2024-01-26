#pragma once
#include "game-element.h"
#include <raylib.h>

class sfx : public game_element {
public:
    sfx(Wave sound);
    sfx(const sfx&) = default;
    sfx(sfx&&) = delete;
    sfx& operator=(const sfx&) = default;
    sfx& operator=(sfx&&) = delete;
    ~sfx() override;

    void play();
    void stop();

    static Wave sfx_1;
    static Wave sfx_2;

private:
    Sound sound_{(Sound){nullptr}};
};
