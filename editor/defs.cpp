#include "defs.h"
#include <raylib.h>

Texture2D defs::atlas_ = (Texture2D){0};

void defs::initialize() {
    defs::atlas_ = LoadTexture(SPRITESHEET_FILE);
}

Texture2D defs::get_spritesheet() {
    return defs::atlas_;
}
