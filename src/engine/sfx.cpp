#include "sfx.h"
#include <raylib.h>

Wave sfx::sfx_1 = (Wave){0};
Wave sfx::sfx_2 = (Wave){0};

void sfx::initialize() {
    sfx::sfx_1 = LoadWave(SFX_1);
    sfx::sfx_2 = LoadWave(SFX_2);
}

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
