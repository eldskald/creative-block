#version 100

precision mediump float;

varying vec2 uv;
uniform sampler2D texture0;

uniform vec4 mainMask;
uniform vec4 shadowMask;
uniform vec4 bgMask;
uniform vec4 debug1;
uniform vec4 debug2;
uniform vec4 debug3;
uniform vec4 debug4;

const vec4 initMainColor = vec4(0.6, 1.0, 1.0, 1.0);
const vec4 initShadowColor = vec4(0.8, 0.0, 1.0, 1.0);
const vec4 initBgColor = vec4(0.3, 0.5, 0.5, 1.0);

void main() {
    vec4 col = vec4(0.0);
    vec4 sample = texture2D(texture0, uv);
    col += step(dot(sample - mainMask, sample - mainMask), 0.05) * initMainColor;
    col += step(dot(sample - shadowMask, sample - shadowMask), 0.05) * initShadowColor;
    col += step(dot(sample - bgMask, sample - bgMask), 0.05) * initBgColor;
    col += step(dot(sample - debug1, sample - debug1), 0.05) * debug1;
    col += step(dot(sample - debug2, sample - debug2), 0.05) * debug2;
    col += step(dot(sample - debug3, sample - debug3), 0.05) * debug3;
    col += step(dot(sample - debug4, sample - debug4), 0.05) * debug4;
    gl_FragColor = col;
}
