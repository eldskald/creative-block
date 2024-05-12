#version 100

precision mediump float;

varying vec2 uv;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 tint;

void main() {
    gl_FragColor = texture2D(texture0, uv) * colDiffuse * tint * fract(uv.x * 8.0);
}
