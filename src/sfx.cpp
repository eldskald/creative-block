#include "sfx.h"
#include <raylib.h>

Wave sfx::sfx_1 = (Wave){0};
Wave sfx::sfx_2 = (Wave){0};

sfx::sfx(Wave sound) {
    this->sound_ = LoadSoundFromWave(sound);
}

sfx::~sfx() {
    UnloadSound(this->sound_);
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
