#version 100

precision mediump float;

varying vec2 uv;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 textureSize;

const float C = 221.0;

//       #####
//     #########
//    ###########
//   #############
//  ###############
//  ###############
// #################
// #################
// #################
// #################
// #################
//  ###############
//  ###############
//   #############
//    ###########
//     #########
//       #####

void main() {
    vec4 col =
        texture2D(texture0, uv + vec2(-2, -8) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -8) / textureSize) +
        texture2D(texture0, uv + vec2(0, -8) / textureSize) +
        texture2D(texture0, uv + vec2(1, -8) / textureSize) +
        texture2D(texture0, uv + vec2(2, -8) / textureSize) +

        texture2D(texture0, uv + vec2(-4, -7) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -7) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -7) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -7) / textureSize) +
        texture2D(texture0, uv + vec2(0, -7) / textureSize) +
        texture2D(texture0, uv + vec2(1, -7) / textureSize) +
        texture2D(texture0, uv + vec2(2, -7) / textureSize) +
        texture2D(texture0, uv + vec2(3, -7) / textureSize) +
        texture2D(texture0, uv + vec2(4, -7) / textureSize) +

        texture2D(texture0, uv + vec2(-5, -6) / textureSize) +
        texture2D(texture0, uv + vec2(-4, -6) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -6) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -6) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -6) / textureSize) +
        texture2D(texture0, uv + vec2(0, -6) / textureSize) +
        texture2D(texture0, uv + vec2(1, -6) / textureSize) +
        texture2D(texture0, uv + vec2(2, -6) / textureSize) +
        texture2D(texture0, uv + vec2(3, -6) / textureSize) +
        texture2D(texture0, uv + vec2(4, -6) / textureSize) +
        texture2D(texture0, uv + vec2(5, -6) / textureSize) +

        texture2D(texture0, uv + vec2(-6, -5) / textureSize) +
        texture2D(texture0, uv + vec2(-5, -5) / textureSize) +
        texture2D(texture0, uv + vec2(-4, -5) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -5) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -5) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -5) / textureSize) +
        texture2D(texture0, uv + vec2(0, -5) / textureSize) +
        texture2D(texture0, uv + vec2(1, -5) / textureSize) +
        texture2D(texture0, uv + vec2(2, -5) / textureSize) +
        texture2D(texture0, uv + vec2(3, -5) / textureSize) +
        texture2D(texture0, uv + vec2(4, -5) / textureSize) +
        texture2D(texture0, uv + vec2(5, -5) / textureSize) +
        texture2D(texture0, uv + vec2(6, -5) / textureSize) +

        texture2D(texture0, uv + vec2(-7, -4) / textureSize) +
        texture2D(texture0, uv + vec2(-6, -4) / textureSize) +
        texture2D(texture0, uv + vec2(-5, -4) / textureSize) +
        texture2D(texture0, uv + vec2(-4, -4) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -4) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -4) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -4) / textureSize) +
        texture2D(texture0, uv + vec2(0, -4) / textureSize) +
        texture2D(texture0, uv + vec2(1, -4) / textureSize) +
        texture2D(texture0, uv + vec2(2, -4) / textureSize) +
        texture2D(texture0, uv + vec2(3, -4) / textureSize) +
        texture2D(texture0, uv + vec2(4, -4) / textureSize) +
        texture2D(texture0, uv + vec2(5, -4) / textureSize) +
        texture2D(texture0, uv + vec2(6, -4) / textureSize) +
        texture2D(texture0, uv + vec2(7, -4) / textureSize) +

        texture2D(texture0, uv + vec2(-7, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-6, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-5, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-4, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -3) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -3) / textureSize) +
        texture2D(texture0, uv + vec2(0, -3) / textureSize) +
        texture2D(texture0, uv + vec2(1, -3) / textureSize) +
        texture2D(texture0, uv + vec2(2, -3) / textureSize) +
        texture2D(texture0, uv + vec2(3, -3) / textureSize) +
        texture2D(texture0, uv + vec2(4, -3) / textureSize) +
        texture2D(texture0, uv + vec2(5, -3) / textureSize) +
        texture2D(texture0, uv + vec2(6, -3) / textureSize) +
        texture2D(texture0, uv + vec2(7, -3) / textureSize) +

        texture2D(texture0, uv + vec2(-8, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-7, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-6, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-5, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-4, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -2) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -2) / textureSize) +
        texture2D(texture0, uv + vec2(0, -2) / textureSize) +
        texture2D(texture0, uv + vec2(1, -2) / textureSize) +
        texture2D(texture0, uv + vec2(2, -2) / textureSize) +
        texture2D(texture0, uv + vec2(3, -2) / textureSize) +
        texture2D(texture0, uv + vec2(4, -2) / textureSize) +
        texture2D(texture0, uv + vec2(5, -2) / textureSize) +
        texture2D(texture0, uv + vec2(6, -2) / textureSize) +
        texture2D(texture0, uv + vec2(7, -2) / textureSize) +
        texture2D(texture0, uv + vec2(8, -2) / textureSize) +

        texture2D(texture0, uv + vec2(-8, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-7, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-6, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-5, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-4, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-3, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-2, -1) / textureSize) +
        texture2D(texture0, uv + vec2(-1, -1) / textureSize) +
        texture2D(texture0, uv + vec2(0, -1) / textureSize) +
        texture2D(texture0, uv + vec2(1, -1) / textureSize) +
        texture2D(texture0, uv + vec2(2, -1) / textureSize) +
        texture2D(texture0, uv + vec2(3, -1) / textureSize) +
        texture2D(texture0, uv + vec2(4, -1) / textureSize) +
        texture2D(texture0, uv + vec2(5, -1) / textureSize) +
        texture2D(texture0, uv + vec2(6, -1) / textureSize) +
        texture2D(texture0, uv + vec2(7, -1) / textureSize) +
        texture2D(texture0, uv + vec2(8, -1) / textureSize) +

        texture2D(texture0, uv + vec2(-8, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-7, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-6, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-5, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 0) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 0) / textureSize) +
        texture2D(texture0, uv + vec2(0, 0) / textureSize) +
        texture2D(texture0, uv + vec2(1, 0) / textureSize) +
        texture2D(texture0, uv + vec2(2, 0) / textureSize) +
        texture2D(texture0, uv + vec2(3, 0) / textureSize) +
        texture2D(texture0, uv + vec2(4, 0) / textureSize) +
        texture2D(texture0, uv + vec2(5, 0) / textureSize) +
        texture2D(texture0, uv + vec2(6, 0) / textureSize) +
        texture2D(texture0, uv + vec2(7, 0) / textureSize) +
        texture2D(texture0, uv + vec2(8, 0) / textureSize) +

        texture2D(texture0, uv + vec2(-8, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-7, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-6, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-5, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 1) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 1) / textureSize) +
        texture2D(texture0, uv + vec2(0, 1) / textureSize) +
        texture2D(texture0, uv + vec2(1, 1) / textureSize) +
        texture2D(texture0, uv + vec2(2, 1) / textureSize) +
        texture2D(texture0, uv + vec2(3, 1) / textureSize) +
        texture2D(texture0, uv + vec2(4, 1) / textureSize) +
        texture2D(texture0, uv + vec2(5, 1) / textureSize) +
        texture2D(texture0, uv + vec2(6, 1) / textureSize) +
        texture2D(texture0, uv + vec2(7, 1) / textureSize) +
        texture2D(texture0, uv + vec2(8, 1) / textureSize) +

        texture2D(texture0, uv + vec2(-8, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-7, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-6, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-5, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 2) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 2) / textureSize) +
        texture2D(texture0, uv + vec2(0, 2) / textureSize) +
        texture2D(texture0, uv + vec2(1, 2) / textureSize) +
        texture2D(texture0, uv + vec2(2, 2) / textureSize) +
        texture2D(texture0, uv + vec2(3, 2) / textureSize) +
        texture2D(texture0, uv + vec2(4, 2) / textureSize) +
        texture2D(texture0, uv + vec2(5, 2) / textureSize) +
        texture2D(texture0, uv + vec2(6, 2) / textureSize) +
        texture2D(texture0, uv + vec2(7, 2) / textureSize) +
        texture2D(texture0, uv + vec2(8, 2) / textureSize) +

        texture2D(texture0, uv + vec2(-7, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-6, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-5, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 3) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 3) / textureSize) +
        texture2D(texture0, uv + vec2(0, 3) / textureSize) +
        texture2D(texture0, uv + vec2(1, 3) / textureSize) +
        texture2D(texture0, uv + vec2(2, 3) / textureSize) +
        texture2D(texture0, uv + vec2(3, 3) / textureSize) +
        texture2D(texture0, uv + vec2(4, 3) / textureSize) +
        texture2D(texture0, uv + vec2(5, 3) / textureSize) +
        texture2D(texture0, uv + vec2(6, 3) / textureSize) +
        texture2D(texture0, uv + vec2(7, 3) / textureSize) +

        texture2D(texture0, uv + vec2(-7, 4) / textureSize) +
        texture2D(texture0, uv + vec2(-6, 4) / textureSize) +
        texture2D(texture0, uv + vec2(-5, 4) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 4) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 4) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 4) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 4) / textureSize) +
        texture2D(texture0, uv + vec2(0, 4) / textureSize) +
        texture2D(texture0, uv + vec2(1, 4) / textureSize) +
        texture2D(texture0, uv + vec2(2, 4) / textureSize) +
        texture2D(texture0, uv + vec2(3, 4) / textureSize) +
        texture2D(texture0, uv + vec2(4, 4) / textureSize) +
        texture2D(texture0, uv + vec2(5, 4) / textureSize) +
        texture2D(texture0, uv + vec2(6, 4) / textureSize) +
        texture2D(texture0, uv + vec2(7, 4) / textureSize) +

        texture2D(texture0, uv + vec2(-6, 5) / textureSize) +
        texture2D(texture0, uv + vec2(-5, 5) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 5) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 5) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 5) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 5) / textureSize) +
        texture2D(texture0, uv + vec2(0, 5) / textureSize) +
        texture2D(texture0, uv + vec2(1, 5) / textureSize) +
        texture2D(texture0, uv + vec2(2, 5) / textureSize) +
        texture2D(texture0, uv + vec2(3, 5) / textureSize) +
        texture2D(texture0, uv + vec2(4, 5) / textureSize) +
        texture2D(texture0, uv + vec2(5, 5) / textureSize) +
        texture2D(texture0, uv + vec2(6, 5) / textureSize) +

        texture2D(texture0, uv + vec2(-5, 6) / textureSize) +
        texture2D(texture0, uv + vec2(-4, 6) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 6) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 6) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 6) / textureSize) +
        texture2D(texture0, uv + vec2(0, 6) / textureSize) +
        texture2D(texture0, uv + vec2(1, 6) / textureSize) +
        texture2D(texture0, uv + vec2(2, 6) / textureSize) +
        texture2D(texture0, uv + vec2(3, 6) / textureSize) +
        texture2D(texture0, uv + vec2(4, 6) / textureSize) +
        texture2D(texture0, uv + vec2(5, 6) / textureSize) +

        texture2D(texture0, uv + vec2(-4, 7) / textureSize) +
        texture2D(texture0, uv + vec2(-3, 7) / textureSize) +
        texture2D(texture0, uv + vec2(-2, 7) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 7) / textureSize) +
        texture2D(texture0, uv + vec2(0, 7) / textureSize) +
        texture2D(texture0, uv + vec2(1, 7) / textureSize) +
        texture2D(texture0, uv + vec2(2, 7) / textureSize) +
        texture2D(texture0, uv + vec2(3, 7) / textureSize) +
        texture2D(texture0, uv + vec2(4, 7) / textureSize) +

        texture2D(texture0, uv + vec2(-2, 8) / textureSize) +
        texture2D(texture0, uv + vec2(-1, 8) / textureSize) +
        texture2D(texture0, uv + vec2(0, 8) / textureSize) +
        texture2D(texture0, uv + vec2(1, 8) / textureSize) +
        texture2D(texture0, uv + vec2(2, 8) / textureSize);

    gl_FragColor = col * colDiffuse / C;
}
