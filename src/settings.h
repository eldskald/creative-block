#pragma once
#include <raylib.h>

namespace game_settings {

inline const char* WINDOW_TITLE = "raylib";
const int WINDOW_SIZE_X = 1024;
const int WINDOW_SIZE_Y = 576;
const int TARGET_FPS = 60;

inline const char* SPRITESHEET_FILE = "assets/textures/spritesheet.png";
const float SPRITESHEET_CELL_SIZE_X = 16.0f;
const float SPRITESHEET_CELL_SIZE_Y = 16.0f;
const int PIXEL_SIZE_X = 4;
const int PIXEL_SIZE_Y = 4;

}; // namespace game_settings
