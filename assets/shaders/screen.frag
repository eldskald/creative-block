#version 100

precision mediump float;

#define TAU 6.283185307179586

#define WAVE_DISPLACE 0.005
#define WAVE_Y_SCALE 4.0

varying vec2 uv;
uniform sampler2D texture0;

uniform vec2 textureSize;

uniform vec4 maskMain;

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
    return clamp((val - min) / (max - min), 0.0, 1.0);
}

void main() {
    vec4 col = vec4(0.0);
    float currLevel = (1.0 - uv.y) * textureSize.y;
    float wLevel = waterLevel;

    // Deciding the water surface line
    wLevel += sin(linstep(wave0.z - wave0.w, wave0.z, abs(uv.x - wave0.y)) * TAU) * wave0.x;
    wLevel += sin(linstep(wave1.z - wave1.w, wave1.z, abs(uv.x - wave1.y)) * TAU) * wave1.x;
    wLevel += sin(linstep(wave2.z - wave2.w, wave2.z, abs(uv.x - wave2.y)) * TAU) * wave2.x;
    wLevel += sin(linstep(wave3.z - wave3.w, wave3.z, abs(uv.x - wave3.y)) * TAU) * wave3.x;
    wLevel += sin(linstep(wave4.z - wave4.w, wave4.z, abs(uv.x - wave4.y)) * TAU) * wave4.x;
    wLevel += sin(linstep(wave5.z - wave5.w, wave5.z, abs(uv.x - wave5.y)) * TAU) * wave5.x;
    wLevel += sin(linstep(wave6.z - wave6.w, wave6.z, abs(uv.x - wave6.y)) * TAU) * wave6.x;
    wLevel += sin(linstep(wave7.z - wave7.w, wave7.z, abs(uv.x - wave7.y)) * TAU) * wave7.x;
    float waterSurface = step(abs(floor(currLevel) - floor(wLevel)), 0.1);
    float waterReflection = step(wLevel, (1.0 - uv.y) * textureSize.y);
    float notWater = (1.0 - waterSurface * water) * (1.0 - waterReflection * water);

    // Drawing the above water
    col += texture2D(texture0, uv) * notWater;

    // Drawing the water reflections
    float wLvNorm = 1.0 - waterLevel / textureSize.y;
    vec2 modUV = vec2(uv.x, 2.0 * wLvNorm - uv.y);
    vec2 wave0Dist = uv - vec2(wave0.y, wLvNorm);
    vec2 wave1Dist = uv - vec2(wave1.y, wLvNorm);
    vec2 wave2Dist = uv - vec2(wave2.y, wLvNorm);
    vec2 wave3Dist = uv - vec2(wave3.y, wLvNorm);
    vec2 wave4Dist = uv - vec2(wave4.y, wLvNorm);
    vec2 wave5Dist = uv - vec2(wave5.y, wLvNorm);
    vec2 wave6Dist = uv - vec2(wave6.y, wLvNorm);
    vec2 wave7Dist = uv - vec2(wave7.y, wLvNorm);
    float t0 = length(vec2(wave0Dist.x, wave0Dist.y * WAVE_Y_SCALE));
    float t1 = length(vec2(wave1Dist.x, wave1Dist.y * WAVE_Y_SCALE));
    float t2 = length(vec2(wave2Dist.x, wave2Dist.y * WAVE_Y_SCALE));
    float t3 = length(vec2(wave3Dist.x, wave3Dist.y * WAVE_Y_SCALE));
    float t4 = length(vec2(wave4Dist.x, wave4Dist.y * WAVE_Y_SCALE));
    float t5 = length(vec2(wave5Dist.x, wave5Dist.y * WAVE_Y_SCALE));
    float t6 = length(vec2(wave6Dist.x, wave6Dist.y * WAVE_Y_SCALE));
    float t7 = length(vec2(wave7Dist.x, wave7Dist.y * WAVE_Y_SCALE));
    float wave0Offset = sin(linstep(wave0.z - wave0.w, wave0.z, t0) * TAU) * WAVE_DISPLACE * wave0.x;
    float wave1Offset = sin(linstep(wave1.z - wave1.w, wave1.z, t1) * TAU) * WAVE_DISPLACE * wave1.x;
    float wave2Offset = sin(linstep(wave2.z - wave2.w, wave2.z, t2) * TAU) * WAVE_DISPLACE * wave2.x;
    float wave3Offset = sin(linstep(wave3.z - wave3.w, wave3.z, t3) * TAU) * WAVE_DISPLACE * wave3.x;
    float wave4Offset = sin(linstep(wave4.z - wave4.w, wave4.z, t4) * TAU) * WAVE_DISPLACE * wave4.x;
    float wave5Offset = sin(linstep(wave5.z - wave5.w, wave5.z, t5) * TAU) * WAVE_DISPLACE * wave5.x;
    float wave6Offset = sin(linstep(wave6.z - wave6.w, wave6.z, t6) * TAU) * WAVE_DISPLACE * wave6.x;
    float wave7Offset = sin(linstep(wave7.z - wave7.w, wave7.z, t7) * TAU) * WAVE_DISPLACE * wave7.x;
    modUV += wave0Offset * normalize(vec2(wave0Dist.x, -wave0Dist.y));
    modUV += wave1Offset * normalize(vec2(wave1Dist.x, -wave1Dist.y));
    modUV += wave2Offset * normalize(vec2(wave2Dist.x, -wave2Dist.y));
    modUV += wave3Offset * normalize(vec2(wave3Dist.x, -wave3Dist.y));
    modUV += wave4Offset * normalize(vec2(wave4Dist.x, -wave4Dist.y));
    modUV += wave5Offset * normalize(vec2(wave5Dist.x, -wave5Dist.y));
    modUV += wave6Offset * normalize(vec2(wave6Dist.x, -wave6Dist.y));
    modUV += wave7Offset * normalize(vec2(wave7Dist.x, -wave7Dist.y));
    col += (texture2D(texture0, modUV) - col) * waterReflection * water;

    // Drawing the water surface
    col += (maskMain - col) * waterSurface * water;

    gl_FragColor = col;
}
