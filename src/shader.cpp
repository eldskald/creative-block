#include "shader.h"
#include <array>
#include <raylib.h>

using namespace std;

Shader shader::base_ = (Shader){0};
Shader shader::screen_ = (Shader){0};
Shader shader::particle_ = (Shader){0};
Shader shader::sample_ = (Shader){0};

void shader::initialize() {

    // Base
    shader::base_ = LoadShader(BASE_VERT_SHADER, BASE_FRAG_SHADER);

    // Screen
    shader::screen_ = LoadShader(BASE_VERT_SHADER, SCREEN_SHADER);
    const float normalize_const = 255.0f;
    Color main = MAIN_MASK_COLOR;
    array<float, 3> main_mask_value = {(float)main.r / normalize_const,
                                       (float)main.g / normalize_const,
                                       (float)main.b / normalize_const};
    int main_mask_loc = GetShaderLocation(shader::screen_, "mainMask");
    SetShaderValue(shader::screen_, main_mask_loc, &main_mask_value, SHADER_UNIFORM_VEC3);
    Color shadow = SHADOW_MASK_COLOR;
    array<float, 3> shadow_mask_value = {(float)shadow.r / normalize_const,
                                         (float)shadow.g / normalize_const,
                                         (float)shadow.b / normalize_const};
    int shadow_mask_loc = GetShaderLocation(shader::screen_, "shadowMask");
    SetShaderValue(shader::screen_, shadow_mask_loc, &shadow_mask_value, SHADER_UNIFORM_VEC3);
    Color bg = BG_MASK_COLOR;
    array<float, 3> bg_mask_value = {(float)bg.r / normalize_const,
                                     (float)bg.g / normalize_const,
                                     (float)bg.b / normalize_const};
    int bg_mask_loc = GetShaderLocation(shader::screen_, "bgMask");
    SetShaderValue(shader::screen_, bg_mask_loc, &bg_mask_value, SHADER_UNIFORM_VEC3);

    // Base particle effects
    shader::particle_ = LoadShader(BASE_VERT_SHADER, SHADER_PARTICLE);
    int shader_rand_loc = GetShaderLocation(shader::particle_, "rand");
    int rand_value = 0;
    SetShaderValue(
        shader::particle_, shader_rand_loc, &rand_value, SHADER_UNIFORM_INT);
    int shader_lifetime_loc = GetShaderLocation(shader::particle_, "lifetime");
    float lifetime_value = 1.0f;
    SetShaderValue(shader::particle_,
                   shader_lifetime_loc,
                   &lifetime_value,
                   SHADER_UNIFORM_FLOAT);

    // Sample
    shader::sample_ = LoadShader(BASE_VERT_SHADER, SHADER_FRAG_1);
    int shader_tint_loc = GetShaderLocation(shader::sample_, "tint");
    array<float, 4> tint_value = {1.0f, 0.0f, 1.0f, 1.0f};
    SetShaderValue(
        shader::sample_, shader_tint_loc, &tint_value, SHADER_UNIFORM_VEC4);
}

Shader* shader::get_base() {
    return &(shader::base_);
}

Shader* shader::get_particle() {
    return &(shader::particle_);
}

Shader* shader::get_sample() {
    return &(shader::sample_);
}

Shader* shader::get_screen() {
    return &(shader::screen_);
}
