#version 100

precision mediump float;

#define POINT_VEL 0.12

#define RADIUS 0.25
#define SMOOTH 0.05

#define MAIN_COL_0 vec4(0.600, 1.000, 1.000, 1.0) // #99ffff
#define MAIN_COL_1 vec4(0.000, 0.800, 0.667, 1.0) // #00ccaa
#define MAIN_COL_2 vec4(0.533, 0.800, 0.000, 1.0) // #88cc00
#define MAIN_COL_3 vec4(0.200, 0.200, 1.000, 1.0) // #3333ff

#define SHAD_COL_0 vec4(0.800, 0.400, 1.000, 1.0) // #cc66ff
#define SHAD_COL_1 vec4(1.000, 0.600, 0.200, 1.0) // #ff9933
#define SHAD_COL_2 vec4(1.000, 0.600, 0.667, 1.0) // #ff99aa
#define SHAD_COL_3 vec4(1.000, 0.918, 0.502, 1.0) // #ffea80

#define P1_INIT vec2(0.28, 0.30)
#define P2_INIT vec2(0.72, 0.70)
#define P3_INIT vec2(0.50, 0.50)

#define P1_DIR normalize(vec2(0.3, -1.0))
#define P2_DIR normalize(vec2(0.3,  1.0))
#define P3_DIR normalize(vec2(1.0,  0.5))

varying vec2 uv;
uniform sampler2D texture0;

uniform vec2 textureSize;
uniform vec2 screenSize;

uniform float time;
uniform float bgColorFactor;
uniform vec4 maskMain;
uniform vec4 maskShadow;
uniform vec4 maskBg;
uniform vec4 debugCol1;
uniform vec4 debugCol2;
uniform vec4 debugCol3;
uniform vec4 debugCol4;

vec2 modUvToBase(vec2 coords) {
    return vec2(
        coords.x * textureSize.x / screenSize.x,
        (1.0 - coords.y) * textureSize.y / screenSize.y
    );
}

float getDist(vec2 p) {
    vec2 coords = modUvToBase(uv);
    float xD = min(abs(coords.x - p.x), 1.0 - abs(coords.x - p.x));
    float yD = min(
            abs(coords.y - p.y),
            1.0 - abs(coords.y - p.y)
        );
    return sqrt(xD * xD + yD * yD * screenSize.y / textureSize.y);
}

void main() {
    vec2 p1 = fract(P1_INIT + P1_DIR * POINT_VEL * time);
    vec2 p2 = fract(P2_INIT + P2_DIR * POINT_VEL * time);
    vec2 p3 = fract(P3_INIT + P3_DIR * POINT_VEL * time);

    float p1Str = 1.0 - smoothstep(RADIUS - SMOOTH, RADIUS, getDist(p1));
    float p2Str = 1.0 - smoothstep(RADIUS - SMOOTH, RADIUS, getDist(p2));
    float p3Str = 1.0 - smoothstep(RADIUS - SMOOTH, RADIUS, getDist(p3));

    vec4 mainCol = MAIN_COL_0;
    mainCol = mainCol * (1.0 - p1Str) + MAIN_COL_1 * p1Str;
    mainCol = mainCol * (1.0 - p2Str) + MAIN_COL_2 * p2Str;
    mainCol = mainCol * (1.0 - p3Str) + MAIN_COL_3 * p3Str;

    vec4 shadowCol = SHAD_COL_0;
    shadowCol = shadowCol * (1.0 - p1Str) + SHAD_COL_1 * p1Str;
    shadowCol = shadowCol * (1.0 - p2Str) + SHAD_COL_2 * p2Str;
    shadowCol = shadowCol * (1.0 - p3Str) + SHAD_COL_3 * p3Str;

    vec4 bgCol = vec4(mainCol.x * bgColorFactor,
            mainCol.y * bgColorFactor,
            mainCol.z * bgColorFactor,
            1.0);

    vec4 smp = texture2D(texture0, uv);
    vec4 col = vec4(0.0);

    col += step(dot(smp - maskMain, smp - maskMain), 0.05) * mainCol;
    col += step(dot(smp - maskShadow, smp - maskShadow), 0.05) * shadowCol;
    col += step(dot(smp - maskBg, smp - maskBg), 0.05) * bgCol;

    col += step(dot(smp - debugCol1, smp - debugCol1), 0.05) * debugCol1;
    col += step(dot(smp - debugCol2, smp - debugCol2), 0.05) * debugCol2;
    col += step(dot(smp - debugCol3, smp - debugCol3), 0.05) * debugCol3;
    col += step(dot(smp - debugCol4, smp - debugCol4), 0.05) * debugCol4;

    gl_FragColor = col;
}
