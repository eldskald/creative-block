#version 100

precision mediump float;

varying vec2 uv;
uniform sampler2D texture0;

uniform vec3 mainMask;
uniform vec3 shadowMask;
uniform vec3 bgMask;

const vec3 initMainColor = vec3(0.6, 1.0, 1.0);
const vec3 initShadowColor = vec3(0.8, 0.0, 1.0);
const vec3 initBgColor = vec3(0.3, 0.5, 0.5);

void main() {
    vec3 col = vec3(0.0);
    vec3 sample = texture2D(texture0, uv).rgb;
    col += step(dot(sample - mainMask, sample - mainMask), 0.05) * initMainColor;
    col += step(dot(sample - shadowMask, sample - shadowMask), 0.05) * initShadowColor;
    col += step(dot(sample - bgMask, sample - bgMask), 0.05) * initBgColor;
    gl_FragColor = vec4(col.r, col.g, col.b, 1.0);
}
