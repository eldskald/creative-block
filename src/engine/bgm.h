#pragma once
#include <raylib.h>
#include <vector>

using namespace std;

class game;

class bgm {
    friend class game;

public:
    static void initialize();
    static void unload();

    static void play(int index);

private:
    static vector<Music> bgms_;
    static int current_;

    static void tick_();
};
