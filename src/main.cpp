#include "settings.h"
#include <raylib.h>

int main() {
    InitWindow(
        GameSettings::WINDOW_SIZE_X, GameSettings::WINDOW_SIZE_Y, "raylib");
    SetTargetFPS(GameSettings::FPS);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        // NOLINTNEXTLINE
        DrawText("Hello world!", 190, 200, 20, DARKGREEN);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
