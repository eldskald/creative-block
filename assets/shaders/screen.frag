#version 100

precision mediump float;

varying vec2 uv;
uniform sampler2D texture0;

uniform vec4 mainMask;
uniform vec4 shadowMask;
uniform vec4 bgMask;

const vec4 initMainColor = vec4(0.6, 1.0, 1.0, 1.0);
const vec4 initShadowColor = vec4(0.8, 0.0, 1.0, 1.0);
const vec4 initBgColor = vec4(0.3, 0.5, 0.5, 1.0);

void main() {
    vec4 col = vec4(0.0);
    vec4 sample = texture2D(texture0, uv);
    col += step(dot(sample - mainMask, sample - mainMask), 0.05) * initMainColor;
    col += step(dot(sample - shadowMask, sample - shadowMask), 0.05) * initShadowColor;
    col += step(dot(sample - bgMask, sample - bgMask), 0.05) * initBgColor;
    gl_FragColor = col;
}
