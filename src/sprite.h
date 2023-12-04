#pragma once
#include "game-element.h"
#include "defs.h"
#include <raylib.h>
#include <vector>

using namespace std;

class game;

class sprite : public game_element {
    friend class game;

public:
    sprite();

    void set_texture(int x, int y);
    void set_animation(vector<defs::animation_frame>);

private:
    Rectangle atlas_rec_;
    vector<defs::animation_frame> animation_;
    float curr_anim_time_;
    int curr_anim_frame_;

    static Texture2D atlas_;

    void tick_() override;
};
