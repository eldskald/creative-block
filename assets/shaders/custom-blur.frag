#version 100

precision mediump float;

varying vec2 uv;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 textureSize;

// Weights
const float w1 = 0.50;
const float w2 = 0.50;
const float w3 = 0.50;
const float w4 = 0.42;
const float w5 = 0.34;
const float w6 = 0.24;
const float w7 = 0.12;
const float w8 = 0.02;

//       88888
//     887777788
//    87766666778
//   8776555556778
//  877655444556778
//  876554333455678
// 87655432223455678
// 87654321112345678
// 87654321012345678
// 87654321112345678
// 87655432223455678
//  876554333455678
//  877655444556778
//   8776555556778
//    87766666778
//     887777788
//       88888

void sampleAt(inout vec4 col, vec2 coords, float factor) {
    vec4 s = texture2D(texture0, coords) * factor;
    col = vec4(max(s.r, col.r), max(s.g, col.g), max(s.b, col.b), max(s.a, col.a));
}

void main() {
    vec4 col = vec4(0);

    // Center
    sampleAt(col, uv, 1.0);

    // First layer
    sampleAt(col, uv + vec2(-1, 1) / textureSize, w1);
    sampleAt(col, uv + vec2(0, 1) / textureSize, w1);
    sampleAt(col, uv + vec2(1, 1) / textureSize, w1);
    sampleAt(col, uv + vec2(1, 0) / textureSize, w1);
    sampleAt(col, uv + vec2(1, -1) / textureSize, w1);
    sampleAt(col, uv + vec2(0, -1) / textureSize, w1);
    sampleAt(col, uv + vec2(-1, -1) / textureSize, w1);
    sampleAt(col, uv + vec2(-1, 0) / textureSize, w1);

    // Second layer
    sampleAt(col, uv + vec2(-1, 2) / textureSize, w2);
    sampleAt(col, uv + vec2(0, 2) / textureSize, w2);
    sampleAt(col, uv + vec2(1, 2) / textureSize, w2);
    sampleAt(col, uv + vec2(2, 1) / textureSize, w2);
    sampleAt(col, uv + vec2(2, 0) / textureSize, w2);
    sampleAt(col, uv + vec2(2, -1) / textureSize, w2);
    sampleAt(col, uv + vec2(1, -2) / textureSize, w2);
    sampleAt(col, uv + vec2(0, -2) / textureSize, w2);
    sampleAt(col, uv + vec2(-1, -2) / textureSize, w2);
    sampleAt(col, uv + vec2(-2, -1) / textureSize, w2);
    sampleAt(col, uv + vec2(-2, 0) / textureSize, w2);
    sampleAt(col, uv + vec2(-2, 1) / textureSize, w2);

    // Third layer
    sampleAt(col, uv + vec2(-1, 3) / textureSize, w3);
    sampleAt(col, uv + vec2(0, 3) / textureSize, w3);
    sampleAt(col, uv + vec2(1, 3) / textureSize, w3);
    sampleAt(col, uv + vec2(2, 2) / textureSize, w3);
    sampleAt(col, uv + vec2(3, 1) / textureSize, w3);
    sampleAt(col, uv + vec2(3, 0) / textureSize, w3);
    sampleAt(col, uv + vec2(3, -1) / textureSize, w3);
    sampleAt(col, uv + vec2(2, -2) / textureSize, w3);
    sampleAt(col, uv + vec2(1, -3) / textureSize, w3);
    sampleAt(col, uv + vec2(0, -3) / textureSize, w3);
    sampleAt(col, uv + vec2(-1, -3) / textureSize, w3);
    sampleAt(col, uv + vec2(-2, -2) / textureSize, w3);
    sampleAt(col, uv + vec2(-3, -1) / textureSize, w3);
    sampleAt(col, uv + vec2(-3, 0) / textureSize, w3);
    sampleAt(col, uv + vec2(-3, 1) / textureSize, w3);
    sampleAt(col, uv + vec2(-2, 2) / textureSize, w3);

    // Fourth layer
    sampleAt(col, uv + vec2(-1, 4) / textureSize, w4);
    sampleAt(col, uv + vec2(0, 4) / textureSize, w4);
    sampleAt(col, uv + vec2(1, 4) / textureSize, w4);
    sampleAt(col, uv + vec2(2, 3) / textureSize, w4);
    sampleAt(col, uv + vec2(3, 2) / textureSize, w4);
    sampleAt(col, uv + vec2(4, 1) / textureSize, w4);
    sampleAt(col, uv + vec2(4, 0) / textureSize, w4);
    sampleAt(col, uv + vec2(4, -1) / textureSize, w4);
    sampleAt(col, uv + vec2(3, -2) / textureSize, w4);
    sampleAt(col, uv + vec2(2, -3) / textureSize, w4);
    sampleAt(col, uv + vec2(1, -4) / textureSize, w4);
    sampleAt(col, uv + vec2(0, -4) / textureSize, w4);
    sampleAt(col, uv + vec2(-1, -4) / textureSize, w4);
    sampleAt(col, uv + vec2(-2, -3) / textureSize, w4);
    sampleAt(col, uv + vec2(-3, -2) / textureSize, w4);
    sampleAt(col, uv + vec2(-4, -1) / textureSize, w4);
    sampleAt(col, uv + vec2(-4, 0) / textureSize, w4);
    sampleAt(col, uv + vec2(-4, 1) / textureSize, w4);
    sampleAt(col, uv + vec2(-3, 2) / textureSize, w4);
    sampleAt(col, uv + vec2(-2, 3) / textureSize, w4);
  
    // Fifth layer
    sampleAt(col, uv + vec2(-2, 5) / textureSize, w5);
    sampleAt(col, uv + vec2(-1, 5) / textureSize, w5);
    sampleAt(col, uv + vec2(0, 5) / textureSize, w5);
    sampleAt(col, uv + vec2(1, 5) / textureSize, w5);
    sampleAt(col, uv + vec2(2, 5) / textureSize, w5);
    sampleAt(col, uv + vec2(2, 4) / textureSize, w5);
    sampleAt(col, uv + vec2(3, 4) / textureSize, w5);
    sampleAt(col, uv + vec2(3, 3) / textureSize, w5);
    sampleAt(col, uv + vec2(4, 3) / textureSize, w5);
    sampleAt(col, uv + vec2(4, 2) / textureSize, w5);
    sampleAt(col, uv + vec2(5, 2) / textureSize, w5);
    sampleAt(col, uv + vec2(5, 1) / textureSize, w5);
    sampleAt(col, uv + vec2(5, 0) / textureSize, w5);
    sampleAt(col, uv + vec2(5, -1) / textureSize, w5);
    sampleAt(col, uv + vec2(5, -2) / textureSize, w5);
    sampleAt(col, uv + vec2(4, -2) / textureSize, w5);
    sampleAt(col, uv + vec2(4, -3) / textureSize, w5);
    sampleAt(col, uv + vec2(3, -3) / textureSize, w5);
    sampleAt(col, uv + vec2(3, -4) / textureSize, w5);
    sampleAt(col, uv + vec2(2, -4) / textureSize, w5);
    sampleAt(col, uv + vec2(2, -5) / textureSize, w5);
    sampleAt(col, uv + vec2(1, -5) / textureSize, w5);
    sampleAt(col, uv + vec2(0, -5) / textureSize, w5);
    sampleAt(col, uv + vec2(-1, -5) / textureSize, w5);
    sampleAt(col, uv + vec2(-2, -5) / textureSize, w5);
    sampleAt(col, uv + vec2(-2, -4) / textureSize, w5);
    sampleAt(col, uv + vec2(-3, -4) / textureSize, w5);
    sampleAt(col, uv + vec2(-3, -3) / textureSize, w5);
    sampleAt(col, uv + vec2(-4, -3) / textureSize, w5);
    sampleAt(col, uv + vec2(-4, -2) / textureSize, w5);
    sampleAt(col, uv + vec2(-5, -2) / textureSize, w5);
    sampleAt(col, uv + vec2(-5, -1) / textureSize, w5);
    sampleAt(col, uv + vec2(-5, 0) / textureSize, w5);
    sampleAt(col, uv + vec2(-5, 1) / textureSize, w5);
    sampleAt(col, uv + vec2(-5, 2) / textureSize, w5);
    sampleAt(col, uv + vec2(-4, 2) / textureSize, w5);
    sampleAt(col, uv + vec2(-4, 3) / textureSize, w5);
    sampleAt(col, uv + vec2(-3, 4) / textureSize, w5);
    sampleAt(col, uv + vec2(-2, 4) / textureSize, w5);

    // Sixth layer
    sampleAt(col, uv + vec2(-2, 6) / textureSize, w6);
    sampleAt(col, uv + vec2(-1, 6) / textureSize, w6);
    sampleAt(col, uv + vec2(0, 6) / textureSize, w6);
    sampleAt(col, uv + vec2(1, 6) / textureSize, w6);
    sampleAt(col, uv + vec2(2, 6) / textureSize, w6);
    sampleAt(col, uv + vec2(3, 5) / textureSize, w6);
    sampleAt(col, uv + vec2(4, 4) / textureSize, w6);
    sampleAt(col, uv + vec2(5, 3) / textureSize, w6);
    sampleAt(col, uv + vec2(6, 2) / textureSize, w6);
    sampleAt(col, uv + vec2(6, 1) / textureSize, w6);
    sampleAt(col, uv + vec2(6, 0) / textureSize, w6);
    sampleAt(col, uv + vec2(6, -1) / textureSize, w6);
    sampleAt(col, uv + vec2(6, -2) / textureSize, w6);
    sampleAt(col, uv + vec2(5, -3) / textureSize, w6);
    sampleAt(col, uv + vec2(4, -4) / textureSize, w6);
    sampleAt(col, uv + vec2(3, -5) / textureSize, w6);
    sampleAt(col, uv + vec2(2, -6) / textureSize, w6);
    sampleAt(col, uv + vec2(1, -6) / textureSize, w6);
    sampleAt(col, uv + vec2(0, -6) / textureSize, w6);
    sampleAt(col, uv + vec2(-1, -6) / textureSize, w6);
    sampleAt(col, uv + vec2(-2, -6) / textureSize, w6);
    sampleAt(col, uv + vec2(-3, -5) / textureSize, w6);
    sampleAt(col, uv + vec2(-4, -4) / textureSize, w6);
    sampleAt(col, uv + vec2(-5, -3) / textureSize, w6);
    sampleAt(col, uv + vec2(-6, -2) / textureSize, w6);
    sampleAt(col, uv + vec2(-6, -1) / textureSize, w6);
    sampleAt(col, uv + vec2(-6, 0) / textureSize, w6);
    sampleAt(col, uv + vec2(-6, 1) / textureSize, w6);
    sampleAt(col, uv + vec2(-6, 2) / textureSize, w6);
    sampleAt(col, uv + vec2(-5, 3) / textureSize, w6);
    sampleAt(col, uv + vec2(-4, 4) / textureSize, w6);
    sampleAt(col, uv + vec2(-3, 5) / textureSize, w6);

    // Seventh layer
    sampleAt(col, uv + vec2(-2, 7) / textureSize, w7);
    sampleAt(col, uv + vec2(-1, 7) / textureSize, w7);
    sampleAt(col, uv + vec2(0, 7) / textureSize, w7);
    sampleAt(col, uv + vec2(1, 7) / textureSize, w7);
    sampleAt(col, uv + vec2(2, 7) / textureSize, w7);
    sampleAt(col, uv + vec2(3, 6) / textureSize, w7);
    sampleAt(col, uv + vec2(4, 6) / textureSize, w7);
    sampleAt(col, uv + vec2(4, 5) / textureSize, w7);
    sampleAt(col, uv + vec2(5, 5) / textureSize, w7);
    sampleAt(col, uv + vec2(5, 4) / textureSize, w7);
    sampleAt(col, uv + vec2(6, 4) / textureSize, w7);
    sampleAt(col, uv + vec2(6, 3) / textureSize, w7);
    sampleAt(col, uv + vec2(7, 2) / textureSize, w7);
    sampleAt(col, uv + vec2(7, 1) / textureSize, w7);
    sampleAt(col, uv + vec2(7, 0) / textureSize, w7);
    sampleAt(col, uv + vec2(7, -1) / textureSize, w7);
    sampleAt(col, uv + vec2(7, -2) / textureSize, w7);
    sampleAt(col, uv + vec2(6, -3) / textureSize, w7);
    sampleAt(col, uv + vec2(6, -4) / textureSize, w7);
    sampleAt(col, uv + vec2(5, -4) / textureSize, w7);
    sampleAt(col, uv + vec2(5, -5) / textureSize, w7);
    sampleAt(col, uv + vec2(4, -5) / textureSize, w7);
    sampleAt(col, uv + vec2(4, -6) / textureSize, w7);
    sampleAt(col, uv + vec2(3, -6) / textureSize, w7);
    sampleAt(col, uv + vec2(2, -7) / textureSize, w7);
    sampleAt(col, uv + vec2(1, -7) / textureSize, w7);
    sampleAt(col, uv + vec2(0, -7) / textureSize, w7);
    sampleAt(col, uv + vec2(-1, -7) / textureSize, w7);
    sampleAt(col, uv + vec2(-2, -7) / textureSize, w7);
    sampleAt(col, uv + vec2(-3, -6) / textureSize, w7);
    sampleAt(col, uv + vec2(-4, -6) / textureSize, w7);
    sampleAt(col, uv + vec2(-4, -5) / textureSize, w7);
    sampleAt(col, uv + vec2(-5, -5) / textureSize, w7);
    sampleAt(col, uv + vec2(-5, -4) / textureSize, w7);
    sampleAt(col, uv + vec2(-6, -4) / textureSize, w7);
    sampleAt(col, uv + vec2(-6, -3) / textureSize, w7);
    sampleAt(col, uv + vec2(-7, -2) / textureSize, w7);
    sampleAt(col, uv + vec2(-7, -1) / textureSize, w7);
    sampleAt(col, uv + vec2(-7, 0) / textureSize, w7);
    sampleAt(col, uv + vec2(-7, 1) / textureSize, w7);
    sampleAt(col, uv + vec2(-7, 2) / textureSize, w7);
    sampleAt(col, uv + vec2(-6, 3) / textureSize, w7);
    sampleAt(col, uv + vec2(-6, 4) / textureSize, w7);
    sampleAt(col, uv + vec2(-5, 4) / textureSize, w7);
    sampleAt(col, uv + vec2(-5, 5) / textureSize, w7);
    sampleAt(col, uv + vec2(-4, 5) / textureSize, w7);
    sampleAt(col, uv + vec2(-4, 6) / textureSize, w7);
    sampleAt(col, uv + vec2(-3, 6) / textureSize, w7);

    // Eighth layer
    sampleAt(col, uv + vec2(-2, 8) / textureSize, w8);
    sampleAt(col, uv + vec2(-1, 8) / textureSize, w8);
    sampleAt(col, uv + vec2(0, 8) / textureSize, w8);
    sampleAt(col, uv + vec2(1, 8) / textureSize, w8);
    sampleAt(col, uv + vec2(2, 8) / textureSize, w8);
    sampleAt(col, uv + vec2(3, 7) / textureSize, w8);
    sampleAt(col, uv + vec2(4, 7) / textureSize, w8);
    sampleAt(col, uv + vec2(5, 6) / textureSize, w8);
    sampleAt(col, uv + vec2(6, 5) / textureSize, w8);
    sampleAt(col, uv + vec2(7, 4) / textureSize, w8);
    sampleAt(col, uv + vec2(7, 3) / textureSize, w8);
    sampleAt(col, uv + vec2(8, 2) / textureSize, w8);
    sampleAt(col, uv + vec2(8, 1) / textureSize, w8);
    sampleAt(col, uv + vec2(8, 0) / textureSize, w8);
    sampleAt(col, uv + vec2(8, -1) / textureSize, w8);
    sampleAt(col, uv + vec2(8, -2) / textureSize, w8);
    sampleAt(col, uv + vec2(7, -3) / textureSize, w8);
    sampleAt(col, uv + vec2(7, -4) / textureSize, w8);
    sampleAt(col, uv + vec2(6, -5) / textureSize, w8);
    sampleAt(col, uv + vec2(5, -6) / textureSize, w8);
    sampleAt(col, uv + vec2(4, -7) / textureSize, w8);
    sampleAt(col, uv + vec2(3, -7) / textureSize, w8);
    sampleAt(col, uv + vec2(2, -8) / textureSize, w8);
    sampleAt(col, uv + vec2(1, -8) / textureSize, w8);
    sampleAt(col, uv + vec2(0, -8) / textureSize, w8);
    sampleAt(col, uv + vec2(-1, -8) / textureSize, w8);
    sampleAt(col, uv + vec2(-2, -8) / textureSize, w8);
    sampleAt(col, uv + vec2(-3, -7) / textureSize, w8);
    sampleAt(col, uv + vec2(-4, -7) / textureSize, w8);
    sampleAt(col, uv + vec2(-5, -6) / textureSize, w8);
    sampleAt(col, uv + vec2(-6, -5) / textureSize, w8);
    sampleAt(col, uv + vec2(-7, -4) / textureSize, w8);
    sampleAt(col, uv + vec2(-7, -3) / textureSize, w8);
    sampleAt(col, uv + vec2(-8, -2) / textureSize, w8);
    sampleAt(col, uv + vec2(-8, -1) / textureSize, w8);
    sampleAt(col, uv + vec2(-8, 0) / textureSize, w8);
    sampleAt(col, uv + vec2(-8, 1) / textureSize, w8);
    sampleAt(col, uv + vec2(-8, 2) / textureSize, w8);
    sampleAt(col, uv + vec2(-7, 3) / textureSize, w8);
    sampleAt(col, uv + vec2(-7, 4) / textureSize, w8);
    sampleAt(col, uv + vec2(-6, 5) / textureSize, w8);
    sampleAt(col, uv + vec2(-5, 6) / textureSize, w8);
    sampleAt(col, uv + vec2(-4, 7) / textureSize, w8);
    sampleAt(col, uv + vec2(-3, 7) / textureSize, w8);

    gl_FragColor = col * colDiffuse;
}
