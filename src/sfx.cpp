#include "sfx.h"
#include <raylib.h>

Sound sfx::sfx_1 = (Sound){0};

sfx::sfx(Sound sound) {
    this->sound_ = sound;
}

void sfx::play() {
    PlaySound(this->sound_);
}

void sfx::stop() {
    StopSound(this->sound_);
}

void sfx::update_pos_() {
    return;
}
