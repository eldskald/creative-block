#version 100

precision mediump float;

varying vec2 uv;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 textureSize;

const float C = 69.0;

//   #####
//  ####### 
// #########
// #########
// #########
// #########
// #########
//  ####### 
//   #####

void main() {
    vec4 col =
        // center
        texture2D(texture0, uv) +

        // layer around the center
        texture2D(texture0, uv + vec2(1, 1) / textureSize) +
        texture2D(texture0, uv + vec2(1, 0) / textureSize) +
        texture2D(texture0, uv + vec2(1, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -1) / textureSize) +
        texture2D(texture0, uv + vec2(0, 1) / textureSize) +
        texture2D(texture0, uv + vec2(0, -1) / textureSize) +

        // second layer
        texture2D(texture0, uv + vec2(2, 2) / textureSize) +
        texture2D(texture0, uv + vec2(2, 1) / textureSize) +
        texture2D(texture0, uv + vec2(2, 0) / textureSize) +
        texture2D(texture0, uv + vec2(2, -1) / textureSize) +
        texture2D(texture0, uv + vec2(2, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -2) / textureSize) +
        texture2D(texture0, uv + vec2(1, 2) / textureSize) +
        texture2D(texture0, uv + vec2(0, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 2) / textureSize) +
        texture2D(texture0, uv + vec2(1, -2) / textureSize) +
        texture2D(texture0, uv + vec2(0, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -2) / textureSize) +

        // third layer
        texture2D(texture0, uv + vec2(3, 3) / textureSize) +
        texture2D(texture0, uv + vec2(3, 2) / textureSize) +
        texture2D(texture0, uv + vec2(3, 1) / textureSize) +
        texture2D(texture0, uv + vec2(3, 0) / textureSize) +
        texture2D(texture0, uv + vec2(3, -1) / textureSize) +
        texture2D(texture0, uv + vec2(3, -2) / textureSize) +
        texture2D(texture0, uv + vec2(3, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -3) / textureSize) +
        texture2D(texture0, uv + vec2(2, 3) / textureSize) +
        texture2D(texture0, uv + vec2(1, 3) / textureSize) +
        texture2D(texture0, uv + vec2(0, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 3) / textureSize) +
        texture2D(texture0, uv + vec2(2, -3) / textureSize) +
        texture2D(texture0, uv + vec2(1, -3) / textureSize) +
        texture2D(texture0, uv + vec2(0, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -3) / textureSize) +

        // fourth layer
        texture2D(texture0, uv + vec2(4, 2) / textureSize) +
        texture2D(texture0, uv + vec2(4, 1) / textureSize) +
        texture2D(texture0, uv + vec2(4, 0) / textureSize) +
        texture2D(texture0, uv + vec2(4, -1) / textureSize) +
        texture2D(texture0, uv + vec2(4, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-4, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-4, -2) / textureSize) +
        texture2D(texture0, uv + vec2(2, -4) / textureSize) +
        texture2D(texture0, uv + vec2(1, -4) / textureSize) +
        texture2D(texture0, uv + vec2(0, -4) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -4) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -4) / textureSize) +
        texture2D(texture0, uv + vec2(2, 4) / textureSize) +
        texture2D(texture0, uv + vec2(1, 4) / textureSize) +
        texture2D(texture0, uv + vec2(0, 4) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 4) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 4) / textureSize);

    gl_FragColor = col * colDiffuse / C;
}
