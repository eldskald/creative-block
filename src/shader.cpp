#include "shader.h"
#include <array>
#include <raylib.h>

using namespace std;

Shader shader::base_ = (Shader){0};
Shader shader::sample_ = (Shader){0};

void shader::initialize() {

    // Base
    shader::base_ = LoadShader(BASE_VERT_SHADER, BASE_FRAG_SHADER);

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

Shader* shader::get_sample() {
    return &(shader::sample_);
}
