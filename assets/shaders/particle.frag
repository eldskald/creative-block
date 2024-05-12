#version 100

precision mediump float;

varying vec2 uv;

uniform int rand;
uniform float lifetime;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main() {
    gl_FragColor = vec4(lifetime, 1.0 - lifetime, 0.5, 1.0) * texture2D(texture0, uv) * colDiffuse;
}
