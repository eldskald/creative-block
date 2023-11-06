#include "sprite.h"
#include <raylib.h>
#include <raymath.h>

sprite::sprite() {
    this->texture_ = {0};
}

void sprite::set_texture(const char* file) {
    this->texture_ = LoadTexture(file);
}

void sprite::tick_() {
    DrawTextureV(texture_, this->global_pos_, WHITE);
}
