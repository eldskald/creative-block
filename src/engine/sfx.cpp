#include "engine/sfx.h"
#include "defs.h"
#include "imports.h"
#include <raylib.h>
#include <unordered_map>

unordered_map<sfx::sound, const char*> sfx::paths_ = SFX_PATHS;
unordered_map<sfx::sound, Sound> sfx::sounds_;
unordered_map<sfx::sound, float> sfx::limit_timers_;

void sfx::initialize() {
    for (sfx::sound sound : SFX_ENUMS) {
        Wave wave = LoadWave(sfx::paths_[sound]);
        sfx::sounds_[sound] = LoadSoundFromWave(wave);
        sfx::limit_timers_[sound] = SFX_LIMIT_TIMER * 2;
        UnloadWave(wave);
    }
}

void sfx::unload() {
    for (sfx::sound sound : SFX_ENUMS) {
        UnloadSound(sfx::sounds_[sound]);
    }
}

void sfx::play(sfx::sound sound) {
    if (sfx::limit_timers_[sound] > SFX_LIMIT_TIMER) {
        StopSound(sfx::sounds_[sound]);
        PlaySound(sfx::sounds_[sound]);
        sfx::limit_timers_[sound] = 0.0f;
    }
}

void sfx::stop(sfx::sound sound) {
    StopSound(sfx::sounds_[sound]);
    sfx::limit_timers_[sound] = 0.0f;
}

void sfx::tick_() {
    for (sfx::sound sound : SFX_ENUMS) {
        if (sfx::limit_timers_[sound] <= SFX_LIMIT_TIMER) {
            sfx::limit_timers_[sound] += GetFrameTime();
        }
    }
}
