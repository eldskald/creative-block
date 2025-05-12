#pragma once
#include "imports.h"
#include <raylib.h>
#include <unordered_map>

using namespace std;

class game;

class sfx {
    friend class game;

public:
    enum sound SFX_DEF_ENUM;

    static void initialize();
    static void unload();

    static void play(sound sound);
    static void stop(sound sound);

private:
    static unordered_map<sound, Sound> sounds_;
    static unordered_map<sound, const char*> paths_;
    static unordered_map<sound, float> limit_timers_;

    static void tick_();
};
