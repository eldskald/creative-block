#pragma once
#include "game-element.h"
#include "shader.h"
#include <raylib.h>
#include <vector>

using namespace std;

using keyframe = struct keyframe {
    Vector2 atlas_coords;
    float duration;
};

using animation = vector<keyframe>;

class sprite : public game_element {
public:
    Vector2 atlas_coords{(Vector2){0}};
    Color tint{WHITE};
    animation anim;
    float animation_starting_phase{0.0f};
    Shader* shader{shader::get_base()};

    static void initialize();

protected:
    void tick_() override;
    void enter_() override;

private:
    int curr_frame_{0};
    float curr_phase_{0.0f};

    static Texture2D atlas_;
};
