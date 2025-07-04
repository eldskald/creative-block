#include "core/game.h"
#include "defs.h"
#include "imports.h"
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
    emscripten_set_main_loop(game::do_game_loop, 0, 1);
#else
    SetTargetFPS(TARGET_FPS);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    Image icon = LoadImage(APP_ICON);
    SetWindowIcon(icon);
    UnloadImage(icon);
    while (!WindowShouldClose()) {
        game::do_game_loop();
    }
#endif

    game::on_close();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
