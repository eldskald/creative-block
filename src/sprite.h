#pragma once
#include "game-element.h"
#include <raylib.h>
#include <vector>
#include <list>

using namespace std;

using keyframe = struct keyframe {
    Vector2 atlas_coords;
    float duration;
};

using animation = vector<keyframe>;

class renderer;

class sprite : public game_element {
    friend class renderer;

public:
    Vector2 atlas_coords{(Vector2){0}};
    Color tint{WHITE};
    animation anim;
    float animation_starting_phase{0.0f};

    static void initialize();

protected:
    void enter_() override;
    void exit_() override;
    void tick_() override;

private:
    int curr_frame_{0};
    float curr_phase_{0.0f};

    static Texture2D atlas_;
    static list<sprite*> sprites_;

    static void render_sprites_();
};
