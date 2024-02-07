#include "button.h"
#include "defs.h"
#include "editor.h"
#include <iostream>
#include <raylib.h>

void click_test() {
    for (int i = 0; i < 10; i++) {
        cout << (char)i;
    }
    cout << endl;
    for (int i = 10; i < 20; i++) {
        cout << (char)i;
    }
    cout << endl;
    for (int i = 20; i < 30; i++) {
        cout << (char)i;
    }
    cout << endl;
    for (int i = 30; i < 40; i++) {
        cout << (char)i;
    }
    cout << endl;
    for (int i = 40; i < 50; i++) {
        cout << (char)i;
    }
    cout << endl;
    for (int i = 50; i < 60; i++) {
        cout << (char)i;
    }
    cout << endl;
}

int main() {
    InitWindow(EDITOR_WINDOW_SIZE_X, EDITOR_WINDOW_SIZE_Y, EDITOR_WINDOW_TITLE);
    SetTargetFPS(EDITOR_FPS);

    button btn = button();
    btn.label = "test";
    btn.rect = (Rectangle){16, EDITOR_WINDOW_SIZE_Y - 40, 100, 32}; // NOLINT
    btn.on_click = click_test;

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
