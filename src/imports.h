#pragma once

#define SCENES_ENUM                                                            \
    {opening,                                                                  \
     credits,                                                                  \
     level_01,                                                                 \
     level_02,                                                                 \
     level_03,                                                                 \
     level_04,                                                                 \
     level_05,                                                                 \
     level_06,                                                                 \
     level_07,                                                                 \
     level_08,                                                                 \
     level_09,                                                                 \
     level_10}
#define SCENES_PATHS                                                           \
    {{opening, "assets/scenes/opening.dat"},                                   \
     {credits, "assets/scenes/credits.dat"},                                   \
     {level_01, "assets/scenes/01.dat"},                                       \
     {level_02, "assets/scenes/02.dat"},                                       \
     {level_03, "assets/scenes/03.dat"},                                       \
     {level_04, "assets/scenes/04.dat"},                                       \
     {level_05, "assets/scenes/05.dat"},                                       \
     {level_06, "assets/scenes/06.dat"},                                       \
     {level_07, "assets/scenes/07.dat"},                                       \
     {level_08, "assets/scenes/08.dat"},                                       \
     {level_09, "assets/scenes/09.dat"},                                       \
     {level_10, "assets/scenes/10.dat"}}

#define SFX_DEF_ENUM {jump, death, respawn, next_level, select, key, gate}
#define SFX_ENUMS                                                              \
    {sfx::jump,                                                                \
     sfx::death,                                                               \
     sfx::respawn,                                                             \
     sfx::next_level,                                                          \
     sfx::select,                                                              \
     sfx::key,                                                                 \
     sfx::gate}
#define SFX_PATHS                                                              \
    {{sfx::jump, "assets/sfx/jump.wav"},                                       \
     {sfx::death, "assets/sfx/death.wav"},                                     \
     {sfx::respawn, "assets/sfx/respawn.wav"},                                 \
     {sfx::next_level, "assets/sfx/next-level.wav"},                           \
     {sfx::select, "assets/sfx/select.wav"},                                   \
     {sfx::key, "assets/sfx/key.wav"},                                         \
     {sfx::gate, "assets/sfx/gate.wav"}}

#define SPRITESHEET_FILE "assets/textures/spritesheet.png"
#define SPRITESHEET_CELL_SIZE_X 8.0f
#define SPRITESHEET_CELL_SIZE_Y 8.0f

#define BASE_VERT_SHADER "assets/shaders/base.vert"
#define BASE_FRAG_SHADER "assets/shaders/base.frag"
#define SCREEN_SHADER "assets/shaders/screen.frag"
#define FIRST_BLUR_SHADER "assets/shaders/custom-blur.frag"
#define SECOND_BLUR_SHADER "assets/shaders/gaussian-range-4.frag"

#define FONT_TITLE "assets/fonts/nordine/Nordine.otf"
