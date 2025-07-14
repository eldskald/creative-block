#include "engine/bgm.h"
#include "defs.h"
#include "imports.h"
#include <algorithm>
#include <raylib.h>
#include <vector>

float bgm::time_ = 0.0f;
float bgm::volume_ = 0.0f;
int bgm::current_ = 0;
int bgm::next_ = 0;
vector<Music> bgm::bgms_;

void bgm::initialize() {
    for (const char* path : BGM_PATHS) {
        bgm::bgms_.push_back(LoadMusicStream(path));
    }
}

void bgm::unload() {
    for (Music bgm : bgm::bgms_) {
        UnloadMusicStream(bgm);
    }
}

void bgm::play(int index) {
    if (bgm::current_ == index) return;

    bgm::next_ = index;
    bgm::time_ = 0.0f;
    if (bgm::current_ == 0) {
        bgm::current_ = index;
        PlayMusicStream(bgm::bgms_.at(index - 1));
    }
}

void bgm::tick_() {
    if (bgm::current_ != bgm::next_) {
        bgm::volume_ =
            clamp(bgm::volume_ - GetFrameTime() * BGM_VOL / BGM_VOL_DOWN_TIME,
                  0.0f,
                  BGM_VOL);
        bgm::time_ += GetFrameTime();
        SetMusicVolume(bgm::bgms_.at(bgm::current_ - 1), bgm::volume_);
        if (bgm::time_ >= BGM_VOL_DOWN_TIME + BGM_TRANSITION_TIME) {
            StopMusicStream(bgm::bgms_.at(bgm::current_ - 1));
            bgm::current_ = bgm::next_;
            bgm::time_ = 0.0f;
            if (bgm::current_ != 0) {
                PlayMusicStream(bgm::bgms_.at(bgm::current_ - 1));
                SetMusicVolume(bgm::bgms_.at(bgm::current_ - 1), bgm::volume_);
            }
        }
    } else if (bgm::volume_ <= BGM_VOL && bgm::current_ != 0) {
        bgm::volume_ =
            clamp(bgm::volume_ + GetFrameTime() * BGM_VOL / BGM_VOL_UP_TIME,
                  0.0f,
                  BGM_VOL);
        bgm::time_ += GetFrameTime();
        SetMusicVolume(bgm::bgms_.at(bgm::current_ - 1), bgm::volume_);
    }
    if (bgm::current_ != 0) UpdateMusicStream(bgm::bgms_.at(bgm::current_ - 1));
}
