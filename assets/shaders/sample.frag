#version 330

in vec2 uv;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 tint;

out vec4 col;

void main() {
    col = texture(texture0, uv) * colDiffuse * tint * fract(uv.x * 10.0);
}
