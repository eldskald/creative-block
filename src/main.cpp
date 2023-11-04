#include "game-element.h"
#include "game.h"
#include "settings.h"
#include <raylib.h>

int main() {
    InitWindow(
        GameSettings::WINDOW_SIZE_X, GameSettings::WINDOW_SIZE_Y, "raylib");
    SetTargetFPS(GameSettings::TARGET_FPS);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello world!", 190, 200, 20, DARKGREEN);
        EndDrawing();

        Game::do_tick_loop();
    }

    CloseWindow();

    return 0;
}
