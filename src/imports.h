#pragma once

#define SCENES_ENUM {OPENING, CREDITS, LEVEL_01}
#define SCENES_MAP                                                             \
    {{OPENING, "assets/scenes/opening.dat"},                                   \
     {CREDITS, "assets/scenes/credits.dat"},                                   \
     {LEVEL_01, "assets/scenes/01.dat"}}
#define SCENES_ORDER                                                           \
    {                                                                          \
        {OPENING, LEVEL_01}, {LEVEL_01, CREDITS}, {                            \
            CREDITS, OPENING                                                   \
        }                                                                      \
    }

#define SPRITESHEET_FILE "assets/textures/spritesheet.png"
#define SPRITESHEET_CELL_SIZE_X 8.0f
#define SPRITESHEET_CELL_SIZE_Y 8.0f

#define BASE_VERT_SHADER "assets/shaders/base.vert"
#define BASE_FRAG_SHADER "assets/shaders/base.frag"
#define SCREEN_SHADER "assets/shaders/screen.frag"
#define FIRST_BLUR_SHADER "assets/shaders/custom-blur.frag"
#define SECOND_BLUR_SHADER "assets/shaders/gaussian-range-4.frag"

#define SFX_1 "assets/sfx/sfx-1.wav"
#define SFX_2 "assets/sfx/sfx-2.wav"

#define FONT_TITLE "assets/fonts/nordine/Nordine.otf"
#define FONT_LABEL "assets/fonts/medodica/MedodicaRegular.otf"
