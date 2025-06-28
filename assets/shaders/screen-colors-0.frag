#version 100

precision mediump float;

varying vec2 uv;
uniform sampler2D texture0;

uniform float time;
uniform float bgColorFactor;
uniform vec4 maskMain;
uniform vec4 maskShadow;
uniform vec4 maskBg;
uniform vec4 debugCol1;
uniform vec4 debugCol2;
uniform vec4 debugCol3;
uniform vec4 debugCol4;

const vec4 mainCol = vec4(0.6, 1.0, 1.0, 1.0); // #99ffff
const vec4 shadowCol = vec4(0.8, 0.4, 1.0, 1.0); // #cc66ff

void main() {
    vec4 smp = texture2D(texture0, uv);
    vec4 col = vec4(0.0);
    vec4 bgCol = vec4(mainCol.x * bgColorFactor,
            mainCol.y * bgColorFactor,
            mainCol.z * bgColorFactor,
            1.0);

    col += step(dot(smp - maskMain, smp - maskMain), 0.05) * mainCol;
    col += step(dot(smp - maskShadow, smp - maskShadow), 0.05) * shadowCol;
    col += step(dot(smp - maskBg, smp - maskBg), 0.05) * bgCol;

    col += step(dot(smp - debugCol1, smp - debugCol1), 0.05) * debugCol1;
    col += step(dot(smp - debugCol2, smp - debugCol2), 0.05) * debugCol2;
    col += step(dot(smp - debugCol3, smp - debugCol3), 0.05) * debugCol3;
    col += step(dot(smp - debugCol4, smp - debugCol4), 0.05) * debugCol4;

    gl_FragColor = col;
}
