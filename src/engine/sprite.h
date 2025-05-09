#pragma once
#include "engine/game-element.h"
#include <list>
#include <map>
#include <raylib.h>
#include <vector>

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
    int z_index{0};
    animation anim;
    float animation_starting_phase{0.0f};
    bool hidden{false};

    static void initialize();
    static void unload();

protected:
    void enter_() override;
    void exit_() override;
    void tick_() override;

private:
    int curr_frame_{0};
    float curr_phase_{0.0f};

    static Texture2D atlas_;
    static map<int, list<sprite*>> sprites_;

    static void render_sprites_();
};
