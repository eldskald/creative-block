#pragma once

#define WINDOW_TITLE "Creative Block"
#define WINDOW_SIZE_X 320
#define WINDOW_SIZE_Y 180
#define STARTING_WINDOW_SIZE_X 1280
#define STARTING_WINDOW_SIZE_Y 720
#define MAIN_TEX_SIZE 512
#define TARGET_FPS 60

#define MAIN_MASK_COLOR (Color){255, 0, 0, 255}
#define SHADOW_MASK_COLOR (Color){0, 0, 255, 255}
#define BG_MASK_COLOR (Color){0, 255, 0, 255}

#define DEBUG_COLOR_1 (Color){255, 255, 255, 255}
#define DEBUG_COLOR_2 (Color){128, 128, 255, 255}
#define DEBUG_COLOR_3 (Color){255, 128, 128, 255}
#define DEBUG_COLOR_4 (Color){128, 255, 128, 255}

#define DEBUG_CTL_SHOW_AREAS KEY_ONE
#define DEBUG_CTL_SHOW_FIXED_BODIES KEY_TWO
#define DEBUG_CTL_SHOW_KINEMATIC_BODIES KEY_THREE
#define DEBUG_CTL_RENDER_FINAL KEY_Z
#define DEBUG_CTL_RENDER_BLUR_1 KEY_X
#define DEBUG_CTL_RENDER_BLUR_2 KEY_C
#define DEBUG_CTL_RENDER_BASE_1 KEY_V
#define DEBUG_CTL_RENDER_BASE_2 KEY_B

#define TITLE_FONT_SIZE 32
#define TITLE_FONT_SPACING -2
#define LABEL_FONT_SIZE 16
#define LABEL_FONT_SPACING 0

#define GAME_TITLE "CREATIVE BLOCK"

#define OPENING_SCREEN_TITLE_Y 80
#define OPENING_SCREEN_MSG "Press enter to start"
#define OPENING_SCREEN_MSG_Y 110
#define OPENING_SCREEN_MSG_CYCLE_TIME 3.0f

#define CREDITS_SCREEN_TITLE_Y 24
#define CREDITS_SCREEN_MSG "Press enter to return"
#define CREDITS_SCREEN_MSG_Y 160
#define CREDITS_SCREEN_MSG_CYCLE_TIME 3.0f

#define PLAYER_ATLAS_COORDS (Vector2){1.0f, 5.0f}
#define PLAYER_Z_INDEX 5
#define PLAYER_SHADOW_Z_INDEX 4
#define PLAYER_SPEED 80.0f
#define PLAYER_ACCELERATION 200.0f
#define PLAYER_FRICTION 400.0f
#define PLAYER_JUMP_SPEED 300.0f
#define PLAYER_GRAVITY 800.0f
#define PLAYER_MAX_FALL_SPEED 300.0f
#define PLAYER_JUMP_SPEED_CUT 0.5f
#define PLAYER_JUMP_BUFFER 0.15f
#define PLAYER_RESPAWN_TIME 1.0f

#define CONTROLS_ACTIONS {left, right, up, down, jump, shadow, reset, start}

#define CONTROLS_KEYBOARD_LEFT KEY_A, KEY_LEFT
#define CONTROLS_KEYBOARD_RIGHT KEY_D, KEY_RIGHT
#define CONTROLS_KEYBOARD_UP KEY_W, KEY_UP
#define CONTROLS_KEYBOARD_DOWN KEY_S, KEY_DOWN
#define CONTROLS_KEYBOARD_JUMP KEY_J
#define CONTROLS_KEYBOARD_SHADOW KEY_K
#define CONTROLS_KEYBOARD_RESET KEY_BACKSPACE
#define CONTROLS_KEYBOARD_START KEY_ENTER

#define CONTROLS_GAMEPAD_LEFT GAMEPAD_BUTTON_LEFT_FACE_LEFT
#define CONTROLS_GAMEPAD_RIGHT GAMEPAD_BUTTON_LEFT_FACE_RIGHT
#define CONTROLS_GAMEPAD_UP GAMEPAD_BUTTON_LEFT_FACE_UP
#define CONTROLS_GAMEPAD_DOWN GAMEPAD_BUTTON_LEFT_FACE_DOWN
#define CONTROLS_GAMEPAD_JUMP                                                  \
    GAMEPAD_BUTTON_RIGHT_FACE_UP, GAMEPAD_BUTTON_RIGHT_FACE_DOWN,              \
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT
#define CONTROLS_GAMEPAD_SHADOW                                                \
    GAMEPAD_BUTTON_LEFT_TRIGGER_1, GAMEPAD_BUTTON_LEFT_TRIGGER_2
#define CONTROLS_GAMEPAD_RESET GAMEPAD_BUTTON_MIDDLE_RIGHT
#define CONTROLS_GAMEPAD_START                                                 \
    GAMEPAD_BUTTON_MIDDLE_RIGHT, GAMEPAD_BUTTON_MIDDLE_LEFT

#define CONTROLS_KEYBOARD                                                      \
    {                                                                          \
        {left, {CONTROLS_KEYBOARD_LEFT}},                                      \
        {right, {CONTROLS_KEYBOARD_RIGHT}},                                    \
        {up, {CONTROLS_KEYBOARD_UP}},                                          \
        {down, {CONTROLS_KEYBOARD_DOWN}},                                      \
        {jump, {CONTROLS_KEYBOARD_JUMP}},                                      \
        {shadow, {CONTROLS_KEYBOARD_SHADOW}},                                  \
        {reset, {CONTROLS_KEYBOARD_RESET}},                                    \
        {start, {CONTROLS_KEYBOARD_START}},                                    \
    }

#define CONTROLS_GAMEPAD                                                       \
    {                                                                          \
        {left, {CONTROLS_GAMEPAD_LEFT}},                                       \
        {right, {CONTROLS_GAMEPAD_RIGHT}},                                     \
        {up, {CONTROLS_GAMEPAD_UP}},                                           \
        {down, {CONTROLS_GAMEPAD_DOWN}},                                       \
        {jump, {CONTROLS_GAMEPAD_JUMP}},                                       \
        {shadow, {CONTROLS_GAMEPAD_SHADOW}},                                   \
        {reset, {CONTROLS_GAMEPAD_RESET}},                                     \
        {start, {CONTROLS_GAMEPAD_START}},                                     \
    }

#define COL_LAYER_PLAYER 0b00000001
#define COL_LAYER_SHADOW 0b00000010
#define COL_LAYER_BLOCKS 0b00000100
#define COL_LAYER_ONEWAY 0b00001000

#define COL_MASK_PLAYER 0b00001100
#define COL_MASK_SHADOW 0b00001100
#define COL_MASK_BLOCKS 0b00000000
#define COL_MASK_ONEWAY 0b00000000
#define COL_V_MASK_PLAYER_SHADOW 0b00000011

#define BACKGROUND_Z_INDEX -1

#define PARTICLE_LEAF_MIN_SPAWN_TIME 1
#define PARTICLE_LEAF_MAX_SPAWN_TIME 30
#define PARTICLE_LEAF_ATLAS_COORDS (Vector2){13.0f, 11.0f}
#define PARTICLE_LEAF_COLLISION_BOX (Rectangle){1.0f, 1.0f, 6.0f, 6.0f}
#define PARTICLE_LEAF_COLLISION_MASK 0b00000111
#define PARTICLE_LEAF_BASE_SPEED_X -12.0f
#define PARTICLE_LEAF_BASE_SPEED_Y 9.0f
#define PARTICLE_LEAF_SWAY_AMPLITUDE_X 16.0f
#define PARTICLE_LEAF_SWAY_AMPLITUDE_Y 8.0f
#define PARTICLE_LEAF_SWAY_PERIOD 5.0f

#define PARTICLE_DRIP_SPAWN_TIME 10.0f
#define PARTICLE_DRIP_FRAME_1_ATLAS_COORDS (Vector2){11.0f, 10.0f}
#define PARTICLE_DRIP_FRAME_2_ATLAS_COORDS (Vector2){11.0f, 11.0f}
#define PARTICLE_DRIP_FRAME_2_TIME 2.0f
#define PARTICLE_DRIP_PARTICLE_ATLAS_COORDS (Vector2){12.0f, 11.0f}
#define PARTICLE_DRIP_SUB_PARTICLE_ATLAS_COORDS (Vector2){15.0f, 11.0f}
#define PARTICLE_DRIP_PARTICLE_COLLISION_BOX (Rectangle){2.0f, 2.0f, 3.0f, 4.0f}
#define PARTICLE_DRIP_SUB_PARTICLE_COLLISION_BOX                               \
    (Rectangle){3.0f, 3.0f, 2.0f, 2.0f}
#define PARTICLE_DRIP_COLLISION_MASK 0b00000111
#define PARTICLE_DRIP_GRAVITY 800.0f
#define PARTICLE_DRIP_MAX_FALLING_SPEED 300.0f
#define PARTICLE_DRIP_SUB_PARTICLE_VEL_1 (Vector2){100.0f, -80.0f}
#define PARTICLE_DRIP_SUB_PARTICLE_VEL_2 (Vector2){40.0f, -140.0f}
#define PARTICLE_DRIP_SUB_PARTICLE_VEL_3 (Vector2){-100.0f, -80.0f}
#define PARTICLE_DRIP_SUB_PARTICLE_VEL_4 (Vector2){-40.0f, -140.0f}

#define PARTICLE_PUFF_MIN_SPAWN_TIME 2
#define PARTICLE_PUFF_MAX_SPAWN_TIME 14
#define PARTICLE_PUFF_ATLAS_COORDS (Vector2){9.0f, 15.0f}
#define PARTICLE_PUFF_COLLISION_BOX (Rectangle){0.0f, 0.0f, 0.0f, 0.0f}
#define PARTICLE_PUFF_COLLISION_MASK 0b00000000
#define PARTICLE_PUFF_VERT_SPEED -9.0f
#define PARTICLE_PUFF_PERIOD 6.0f
#define PARTICLE_PUFF_AMPLITUDE 6.0f
#define PARTICLE_PUFF_MIN_LIFETIME 5.0f
#define PARTICLE_PUFF_MAX_LIFETIME 15.0f

#define PARTICLE_DEATH_VEL_1 (Vector2){40.0f, -320.0f}
#define PARTICLE_DEATH_VEL_2 (Vector2){120.0f, -200.0f}
#define PARTICLE_DEATH_VEL_3 (Vector2){200.0f, -120.0f}
#define PARTICLE_DEATH_VEL_4 (Vector2){80.0f, 0.0f}
#define PARTICLE_DEATH_VEL_5 (Vector2){-80.0f, 0.0f}
#define PARTICLE_DEATH_VEL_6 (Vector2){-200.0f, -120.0f}
#define PARTICLE_DEATH_VEL_7 (Vector2){-120.0f, -200.0f}
#define PARTICLE_DEATH_VEL_8 (Vector2){-40.0f, -320.0f}
#define PARTICLE_DEATH_COLLISION_BOX (Rectangle){2.0f, 2.0f, 4.0f, 4.0f}
#define PARTICLE_DEATH_COLLISION_MASK 0b00000100
#define PARTICLE_DEATH_ATLAS_COORDS (Vector2){14.0f, 11.0f}
#define PARTICLE_DEATH_MAX_BOUNCES 4
#define PARTICLE_DEATH_BOUNCE_ELASTICITY 0.6f
#define PARTICLE_DEATH_GRAVITY 800.0f

#define PARTICLE_RESPAWN_VEL_1 (Vector2){20.0f, -160.0f}
#define PARTICLE_RESPAWN_VEL_2 (Vector2){60.0f, -100.0f}
#define PARTICLE_RESPAWN_VEL_3 (Vector2){100.0f, -60.0f}
#define PARTICLE_RESPAWN_VEL_4 (Vector2){40.0f, 0.0f}
#define PARTICLE_RESPAWN_VEL_5 (Vector2){-40.0f, 0.0f}
#define PARTICLE_RESPAWN_VEL_6 (Vector2){-100.0f, -60.0f}
#define PARTICLE_RESPAWN_VEL_7 (Vector2){-60.0f, -100.0f}
#define PARTICLE_RESPAWN_VEL_8 (Vector2){-20.0f, -160.0f}
#define PARTICLE_RESPAWN_COLLISION_BOX (Rectangle){3.0f, 3.0f, 2.0f, 2.0f}
#define PARTICLE_RESPAWN_COLLISION_MASK 0b00000100
#define PARTICLE_RESPAWN_ATLAS_COORDS (Vector2){15.0f, 11.0f}
#define PARTICLE_RESPAWN_MAX_BOUNCES 4
#define PARTICLE_RESPAWN_BOUNCE_ELASTICITY 0.8f
#define PARTICLE_RESPAWN_GRAVITY 800.0f
