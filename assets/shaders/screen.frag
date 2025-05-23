#version 100

precision mediump float;

#define TAU 6.283185307179586

varying vec2 uv;
uniform sampler2D texture0;

uniform vec2 textureSize;
uniform float time;

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

uniform float water;
uniform float waterLevel;
uniform vec4 wave0;
uniform vec4 wave1;
uniform vec4 wave2;
uniform vec4 wave3;
uniform vec4 wave4;
uniform vec4 wave5;
uniform vec4 wave6;
uniform vec4 wave7;

float linstep(float min, float max, float val) {
    return clamp((val - min)/(max - min), 0.0, 1.0);
}

void main() {
    vec4 col = vec4(0.0);
    vec4 sample = texture2D(texture0, uv);

    float currLevel = (1.0 - uv.y) * textureSize.y;
    float wLevel = waterLevel;

    wLevel += sin(linstep(wave0.z - wave0.w, wave0.z, abs(uv.x - wave0.y)) * TAU) * wave0.x;
    wLevel += sin(linstep(wave1.z - wave1.w, wave1.z, abs(uv.x - wave1.y)) * TAU) * wave1.x;
    wLevel += sin(linstep(wave2.z - wave2.w, wave2.z, abs(uv.x - wave2.y)) * TAU) * wave2.x;
    wLevel += sin(linstep(wave3.z - wave3.w, wave3.z, abs(uv.x - wave3.y)) * TAU) * wave3.x;
    wLevel += sin(linstep(wave4.z - wave4.w, wave4.z, abs(uv.x - wave4.y)) * TAU) * wave4.x;
    wLevel += sin(linstep(wave5.z - wave5.w, wave5.z, abs(uv.x - wave5.y)) * TAU) * wave5.x;
    wLevel += sin(linstep(wave6.z - wave6.w, wave6.z, abs(uv.x - wave6.y)) * TAU) * wave6.x;
    wLevel += sin(linstep(wave7.z - wave7.w, wave7.z, abs(uv.x - wave7.y)) * TAU) * wave7.x;

    float waterSurface = step(abs(floor(currLevel) - floor(wLevel)), 0.1);
    float underWater = step(wLevel, (1.0 - uv.y) * textureSize.y);
    float notWater = (1.0 - waterSurface * water) * (1.0 - underWater * water);

    col += initMainColor * waterSurface * water;

    col += step(dot(sample - mainMask, sample - mainMask), 0.05) * initMainColor * notWater;
    col += step(dot(sample - shadowMask, sample - shadowMask), 0.05) * initShadowColor * notWater;
    col += step(dot(sample - bgMask, sample - bgMask), 0.05) * initBgColor * notWater;
    col += step(dot(sample - debug1, sample - debug1), 0.05) * debug1 * notWater;
    col += step(dot(sample - debug2, sample - debug2), 0.05) * debug2 * notWater;
    col += step(dot(sample - debug3, sample - debug3), 0.05) * debug3 * notWater;
    col += step(dot(sample - debug4, sample - debug4), 0.05) * debug4 * notWater;

    gl_FragColor = col;
}
