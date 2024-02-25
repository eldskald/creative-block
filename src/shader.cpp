#include "shader.h"
#include <array>
#include <raylib.h>

using namespace std;

Shader shader::base_ = (Shader){0};
Shader shader::particle_ = (Shader){0};
Shader shader::sample_ = (Shader){0};

void shader::initialize() {

    // Base
    shader::base_ = LoadShader(BASE_VERT_SHADER, BASE_FRAG_SHADER);

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
