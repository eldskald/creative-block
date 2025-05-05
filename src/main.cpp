#include "core/game.h"
#include <raylib.h>
#include <raymath.h>

#ifdef WEB
#include <emscripten/emscripten.h>
#endif

using namespace std;

int main() {

#ifndef DEV
    SetTraceLogLevel(LOG_NONE);
#endif

    InitWindow(STARTING_WINDOW_SIZE_X, STARTING_WINDOW_SIZE_Y, WINDOW_TITLE);
    InitAudioDevice();
    SetExitKey(KEY_NULL);

    game::on_start();

#ifdef WEB
    emscripten_set_main_loop(game::do_game_loop, TARGET_FPS, 1);
#else
    SetTargetFPS(TARGET_FPS);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    // To force a size change and update window titlebar buttons
    SetWindowSize(STARTING_WINDOW_SIZE_X, STARTING_WINDOW_SIZE_Y);
    while (!WindowShouldClose()) {
        game::do_game_loop();
    }
#endif

    game::on_close();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
