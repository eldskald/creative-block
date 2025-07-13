#include "engine/bgm.h"
#include "imports.h"
#include <iostream>
#include <raylib.h>
#include <vector>

int bgm::current_ = 0;
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
    if (index == bgm::current_) return;

    if (index == 0) {
        StopMusicStream(bgm::bgms_.at(bgm::current_ - 1));
        bgm::current_ = 0;
    } else {
        PlayMusicStream(bgm::bgms_.at(index - 1));
        bgm::current_ = index;
        std::cout << index << std::endl;
    }
}

void bgm::tick_() {
    if (bgm::current_ > 0) UpdateMusicStream(bgm::bgms_.at(bgm::current_ - 1));
}
