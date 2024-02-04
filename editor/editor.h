#pragma once
#include "tilemap.h"
#include "tileset.h"
#include <raylib.h>

class editor {
public:
    static void initialize();
    static void tick();

private:
    static tilemap* tilemap_;
    static tileset* tileset_;
};
