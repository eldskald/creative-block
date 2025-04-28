#version 100

precision mediump float;

varying vec2 uv;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 textureSize;
uniform float intensity;

//    1    8   28   56   70   56   28    8    1
//    8   64  224  448  560  448  224   64    8
//   28  224  784 1568 1960 1568  784  224   28
//   56  448 1568 3136 3920 3136 1568  448   56
//   70  560 1960 3920 4900 3920 1960  560   70
//   56  448 1568 3136 3920 3136 1568  448   56
//   28  224  784 1568 1960 1568  784  224   28
//    8   64  224  448  560  448  224   64    8
//    1    8   28   56   70   56   28    8    1
// 1 / 65536

void main() {
    vec4 col =
        texture2D(texture0, uv + vec2(-4, -4) / textureSize) * 1.0 +
        texture2D(texture0, uv + vec2(-3, -4) / textureSize) * 8.0 +
        texture2D(texture0, uv + vec2(-2, -4) / textureSize) * 28.0 +
        texture2D(texture0, uv + vec2(-1, -4) / textureSize) * 56.0 +
        texture2D(texture0, uv + vec2(0, -4) / textureSize) * 70.0 +
        texture2D(texture0, uv + vec2(1, -4) / textureSize) * 56.0 +
        texture2D(texture0, uv + vec2(2, -4) / textureSize) * 28.0 +
        texture2D(texture0, uv + vec2(3, -4) / textureSize) * 8.0 +
        texture2D(texture0, uv + vec2(4, -4) / textureSize) * 1.0 +

        texture2D(texture0, uv + vec2(-4, -3) / textureSize) * 8.0 +
        texture2D(texture0, uv + vec2(-3, -3) / textureSize) * 64.0 +
        texture2D(texture0, uv + vec2(-2, -3) / textureSize) * 224.0 +
        texture2D(texture0, uv + vec2(-1, -3) / textureSize) * 448.0 +
        texture2D(texture0, uv + vec2(0, -3) / textureSize) * 560.0 +
        texture2D(texture0, uv + vec2(1, -3) / textureSize) * 448.0 +
        texture2D(texture0, uv + vec2(2, -3) / textureSize) * 224.0 +
        texture2D(texture0, uv + vec2(3, -3) / textureSize) * 64.0 +
        texture2D(texture0, uv + vec2(4, -3) / textureSize) * 8.0 +

        texture2D(texture0, uv + vec2(-4, -2) / textureSize) * 28.0 +
        texture2D(texture0, uv + vec2(-3, -2) / textureSize) * 224.0 +
        texture2D(texture0, uv + vec2(-2, -2) / textureSize) * 784.0 +
        texture2D(texture0, uv + vec2(-1, -2) / textureSize) * 1568.0 +
        texture2D(texture0, uv + vec2(0, -2) / textureSize) * 1960.0 +
        texture2D(texture0, uv + vec2(1, -2) / textureSize) * 1568.0 +
        texture2D(texture0, uv + vec2(2, -2) / textureSize) * 784.0 +
        texture2D(texture0, uv + vec2(3, -2) / textureSize) * 224.0 +
        texture2D(texture0, uv + vec2(4, -2) / textureSize) * 28.0 +

        texture2D(texture0, uv + vec2(-4, -1) / textureSize) * 56.0 +
        texture2D(texture0, uv + vec2(-3, -1) / textureSize) * 448.0 +
        texture2D(texture0, uv + vec2(-2, -1) / textureSize) * 1568.0 +
        texture2D(texture0, uv + vec2(-1, -1) / textureSize) * 3136.0 +
        texture2D(texture0, uv + vec2(0, -1) / textureSize) * 3920.0 +
        texture2D(texture0, uv + vec2(1, -1) / textureSize) * 3136.0 +
        texture2D(texture0, uv + vec2(2, -1) / textureSize) * 1568.0 +
        texture2D(texture0, uv + vec2(3, -1) / textureSize) * 448.0 +
        texture2D(texture0, uv + vec2(4, -1) / textureSize) * 56.0 +

        texture2D(texture0, uv + vec2(-4, 0) / textureSize) * 70.0 +
        texture2D(texture0, uv + vec2(-3, 0) / textureSize) * 560.0 +
        texture2D(texture0, uv + vec2(-2, 0) / textureSize) * 1960.0 +
        texture2D(texture0, uv + vec2(-1, 0) / textureSize) * 3920.0 +
        texture2D(texture0, uv + vec2(0, 0) / textureSize) * 4900.0 +
        texture2D(texture0, uv + vec2(1, 0) / textureSize) * 3920.0 +
        texture2D(texture0, uv + vec2(2, 0) / textureSize) * 1960.0 +
        texture2D(texture0, uv + vec2(3, 0) / textureSize) * 560.0 +
        texture2D(texture0, uv + vec2(4, 0) / textureSize) * 70.0 +

        texture2D(texture0, uv + vec2(-4, 1) / textureSize) * 56.0 +
        texture2D(texture0, uv + vec2(-3, 1) / textureSize) * 448.0 +
        texture2D(texture0, uv + vec2(-2, 1) / textureSize) * 1568.0 +
        texture2D(texture0, uv + vec2(-1, 1) / textureSize) * 3136.0 +
        texture2D(texture0, uv + vec2(0, 1) / textureSize) * 3920.0 +
        texture2D(texture0, uv + vec2(1, 1) / textureSize) * 3136.0 +
        texture2D(texture0, uv + vec2(2, 1) / textureSize) * 1568.0 +
        texture2D(texture0, uv + vec2(3, 1) / textureSize) * 448.0 +
        texture2D(texture0, uv + vec2(4, 1) / textureSize) * 56.0 +

        texture2D(texture0, uv + vec2(-4, 2) / textureSize) * 28.0 +
        texture2D(texture0, uv + vec2(-3, 2) / textureSize) * 224.0 +
        texture2D(texture0, uv + vec2(-2, 2) / textureSize) * 784.0 +
        texture2D(texture0, uv + vec2(-1, 2) / textureSize) * 1568.0 +
        texture2D(texture0, uv + vec2(0, 2) / textureSize) * 1960.0 +
        texture2D(texture0, uv + vec2(1, 2) / textureSize) * 1568.0 +
        texture2D(texture0, uv + vec2(2, 2) / textureSize) * 784.0 +
        texture2D(texture0, uv + vec2(3, 2) / textureSize) * 224.0 +
        texture2D(texture0, uv + vec2(4, 2) / textureSize) * 28.0 +

        texture2D(texture0, uv + vec2(-4, 3) / textureSize) * 8.0 +
        texture2D(texture0, uv + vec2(-3, 3) / textureSize) * 64.0 +
        texture2D(texture0, uv + vec2(-2, 3) / textureSize) * 224.0 +
        texture2D(texture0, uv + vec2(-1, 3) / textureSize) * 448.0 +
        texture2D(texture0, uv + vec2(0, 3) / textureSize) * 560.0 +
        texture2D(texture0, uv + vec2(1, 3) / textureSize) * 448.0 +
        texture2D(texture0, uv + vec2(2, 3) / textureSize) * 224.0 +
        texture2D(texture0, uv + vec2(3, 3) / textureSize) * 64.0 +
        texture2D(texture0, uv + vec2(4, 3) / textureSize) * 8.0 +

        texture2D(texture0, uv + vec2(-4, 4) / textureSize) * 1.0 +
        texture2D(texture0, uv + vec2(-3, 4) / textureSize) * 8.0 +
        texture2D(texture0, uv + vec2(-2, 4) / textureSize) * 28.0 +
        texture2D(texture0, uv + vec2(-1, 4) / textureSize) * 56.0 +
        texture2D(texture0, uv + vec2(0, 4) / textureSize) * 70.0 +
        texture2D(texture0, uv + vec2(1, 4) / textureSize) * 56.0 +
        texture2D(texture0, uv + vec2(2, 4) / textureSize) * 28.0 +
        texture2D(texture0, uv + vec2(3, 4) / textureSize) * 8.0 +
        texture2D(texture0, uv + vec2(4, 4) / textureSize) * 1.0;

    gl_FragColor = col * colDiffuse * intensity / 65536.0;
}
