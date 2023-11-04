#include "game-element.h"
#include "game.h"
#include "settings.h"
#include <raylib.h>

int main() {
    InitWindow(game_settings::WINDOW_SIZE_X,
               game_settings::WINDOW_SIZE_Y,
               game_settings::WINDOW_TITLE);
    SetTargetFPS(game_settings::TARGET_FPS);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello world!", 190, 200, 20, DARKGREEN);
        EndDrawing();

        game::do_tick_loop();
    }

    CloseWindow();

    return 0;
}
