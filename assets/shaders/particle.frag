#version 330

in vec2 uv;

uniform int rand;
uniform float lifetime;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 output;

void main() {
    output = vec4(lifetime, 1.0 - lifetime, 0.5, 1.0) * texture(texture0, uv) * colDiffuse;
}
