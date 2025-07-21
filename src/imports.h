#pragma once

#define SCENES_ENUM                                                            \
    {opening,  level_01, level_02, level_03, level_04, level_05, level_06,     \
     level_07, level_08, level_09, level_10, level_11, level_12, level_13,     \
     level_14, level_15, level_16, level_17, level_18, level_19, level_20,     \
     level_21, level_22, level_23, level_24, level_25, level_26, level_27,     \
     level_28, level_29, level_30, level_31, level_32, credits}
#define SCENES_PATHS                                                           \
    {{opening, "assets/scenes/opening.dat"},                                   \
     {level_01, "assets/scenes/01.dat"},                                       \
     {level_02, "assets/scenes/02.dat"},                                       \
     {level_03, "assets/scenes/03.dat"},                                       \
     {level_04, "assets/scenes/04.dat"},                                       \
     {level_05, "assets/scenes/05.dat"},                                       \
     {level_06, "assets/scenes/06.dat"},                                       \
     {level_07, "assets/scenes/07.dat"},                                       \
     {level_08, "assets/scenes/08.dat"},                                       \
     {level_09, "assets/scenes/09.dat"},                                       \
     {level_10, "assets/scenes/10.dat"},                                       \
     {level_11, "assets/scenes/11.dat"},                                       \
     {level_12, "assets/scenes/12.dat"},                                       \
     {level_13, "assets/scenes/13.dat"},                                       \
     {level_14, "assets/scenes/14.dat"},                                       \
     {level_15, "assets/scenes/15.dat"},                                       \
     {level_16, "assets/scenes/16.dat"},                                       \
     {level_17, "assets/scenes/17.dat"},                                       \
     {level_18, "assets/scenes/18.dat"},                                       \
     {level_19, "assets/scenes/19.dat"},                                       \
     {level_20, "assets/scenes/20.dat"},                                       \
     {level_21, "assets/scenes/21.dat"},                                       \
     {level_22, "assets/scenes/22.dat"},                                       \
     {level_23, "assets/scenes/23.dat"},                                       \
     {level_24, "assets/scenes/24.dat"},                                       \
     {level_25, "assets/scenes/25.dat"},                                       \
     {level_26, "assets/scenes/26.dat"},                                       \
     {level_27, "assets/scenes/27.dat"},                                       \
     {level_28, "assets/scenes/28.dat"},                                       \
     {level_29, "assets/scenes/29.dat"},                                       \
     {level_30, "assets/scenes/30.dat"},                                       \
     {level_31, "assets/scenes/31.dat"},                                       \
     {level_32, "assets/scenes/32.dat"},                                       \
     {credits, "assets/scenes/credits.dat"}}

#define SFX_DEF_ENUM                                                           \
    {jump,                                                                     \
     shadow_jump,                                                              \
     shadow,                                                                   \
     death,                                                                    \
     shadow_death,                                                             \
     respawn,                                                                  \
     next_level,                                                               \
     select,                                                                   \
     key,                                                                      \
     gate}
#define SFX_ENUMS                                                              \
    {sfx::jump,                                                                \
     sfx::shadow_jump,                                                         \
     sfx::shadow,                                                              \
     sfx::death,                                                               \
     sfx::shadow_death,                                                        \
     sfx::respawn,                                                             \
     sfx::next_level,                                                          \
     sfx::select,                                                              \
     sfx::key,                                                                 \
     sfx::gate}
#define SFX_PATHS                                                              \
    {{sfx::jump, "assets/sfx/jump.wav"},                                       \
     {sfx::shadow_jump, "assets/sfx/shadow-jump.wav"},                         \
     {sfx::shadow, "assets/sfx/shadow.wav"},                                   \
     {sfx::death, "assets/sfx/death.wav"},                                     \
     {sfx::shadow_death, "assets/sfx/shadow-death.wav"},                       \
     {sfx::respawn, "assets/sfx/respawn.wav"},                                 \
     {sfx::next_level, "assets/sfx/next-level.wav"},                           \
     {sfx::select, "assets/sfx/select.wav"},                                   \
     {sfx::key, "assets/sfx/key.wav"},                                         \
     {sfx::gate, "assets/sfx/gate.wav"}}

#define BGM_PATHS                                                              \
    {"assets/bgm/lonely-night.ogg",                                            \
     "assets/bgm/unstable-field.ogg",                                          \
     "assets/bgm/gods-forbid.ogg"}

#define SPRITESHEET_FILE "assets/textures/spritesheet.png"
#define SPRITESHEET_CELL_SIZE_X 8.0f
#define SPRITESHEET_CELL_SIZE_Y 8.0f

#define MAIN_GRADIENT_1_TEXTURE "assets/textures/main-gradient-1.png"
#define MAIN_GRADIENT_2_TEXTURE "assets/textures/main-gradient-2.png"
#define MAIN_GRADIENT_3_TEXTURE "assets/textures/main-gradient-3.png"
#define MAIN_GRADIENT_4_TEXTURE "assets/textures/main-gradient-4.png"
#define SHADOW_GRADIENT_1_TEXTURE "assets/textures/shadow-gradient-1.png"
#define SHADOW_GRADIENT_2_TEXTURE "assets/textures/shadow-gradient-2.png"
#define SHADOW_GRADIENT_3_TEXTURE "assets/textures/shadow-gradient-3.png"
#define SHADOW_GRADIENT_4_TEXTURE "assets/textures/shadow-gradient-4.png"

#define BASE_VERT_SHADER "assets/shaders/base.vert"
#define BASE_FRAG_SHADER "assets/shaders/base.frag"
#define SCREEN_SHADER "assets/shaders/screen.frag"
#define FIRST_BLUR_SHADER "assets/shaders/custom-blur.frag"
#define SECOND_BLUR_SHADER "assets/shaders/gaussian-range-4.frag"

#define SCREEN_COLOR_SHADER_0 "assets/shaders/screen-colors-0.frag"
#define SCREEN_COLOR_SHADER_1 "assets/shaders/screen-colors-1.frag"
#define SCREEN_COLOR_SHADER_2 "assets/shaders/screen-colors-2.frag"

#define FONT_TITLE "assets/fonts/nordine/Nordine.otf"

#define APP_ICON "assets/textures/icon.png"
