#include "button.h"
#include "defs.h"
#include "editor.h"
#include <raylib.h>

int main() {
    InitWindow(EDITOR_WINDOW_SIZE_X, EDITOR_WINDOW_SIZE_Y, EDITOR_WINDOW_TITLE);
    SetTargetFPS(EDITOR_FPS);

    button btn = button();
    btn.label = "test";
    btn.rect = (Rectangle){16, EDITOR_WINDOW_SIZE_Y - 40, 100, 32}; // NOLINT

    editor::initialize();
    while (!WindowShouldClose()) {
        BeginDrawing();
        editor::tick();
        btn.tick();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
