#version 100

precision mediump float;

varying vec2 uv;
uniform sampler2D texture0;

uniform vec2 textureSize;
uniform vec2 direction;

// Gaussian blur kernel weights
// const float weight0 = 0.16;
// const float weight1 = 0.15;
// const float weight2 = 0.12;
// const float weight3 = 0.09;
// const float weight4 = 0.05;

// Voices in my head kernel weights
const float weight0 = 0.1;
const float weight1 = 0.1;
const float weight2 = 0.1;
const float weight3 = 0.09;
const float weight4 = 0.08;
const float weight5 = 0.06;
const float weight6 = 0.04;
const float weight7 = 0.03;
const float weight8 = 0.02;

void main() {
    vec2 offset = direction / textureSize;
    vec4 sum = vec4(0.0);

    sum += texture2D(texture0, uv + 8.0 * offset) * weight8;
    sum += texture2D(texture0, uv + 7.0 * offset) * weight7;
    sum += texture2D(texture0, uv + 6.0 * offset) * weight6;
    sum += texture2D(texture0, uv + 5.0 * offset) * weight5;
    sum += texture2D(texture0, uv + 4.0 * offset) * weight4;
    sum += texture2D(texture0, uv + 3.0 * offset) * weight3;
    sum += texture2D(texture0, uv + 2.0 * offset) * weight2;
    sum += texture2D(texture0, uv + 1.0 * offset) * weight1;
    sum += texture2D(texture0, uv) * weight0;
    sum += texture2D(texture0, uv - 1.0 * offset) * weight1;
    sum += texture2D(texture0, uv - 2.0 * offset) * weight2;
    sum += texture2D(texture0, uv - 3.0 * offset) * weight3;
    sum += texture2D(texture0, uv - 4.0 * offset) * weight4;
    sum += texture2D(texture0, uv - 5.0 * offset) * weight5;
    sum += texture2D(texture0, uv - 6.0 * offset) * weight6;
    sum += texture2D(texture0, uv - 7.0 * offset) * weight7;
    sum += texture2D(texture0, uv - 8.0 * offset) * weight8;

    gl_FragColor = sum;
}
