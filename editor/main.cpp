#include "editor.h"
#include <raylib.h>

int main() {
    InitWindow(EDITOR_WINDOW_SIZE_X, EDITOR_WINDOW_SIZE_Y, EDITOR_WINDOW_TITLE);
    SetTargetFPS(EDITOR_FPS);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Account for the border around the tilemap
        DrawRectangleLinesEx((Rectangle){TILEMAP_ORIGIN_X - 1,
                                         TILEMAP_ORIGIN_Y - 1,
                                         TILEMAP_SIZE_X + 2,
                                         TILEMAP_SIZE_Y + 2},
                             1,
                             BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
