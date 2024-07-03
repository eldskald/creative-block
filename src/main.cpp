#include "game-element.h"
#include "game.h"
#include "physics-body.h"
#include "shader.h"
#include <raylib.h>
#include <raymath.h>

#ifdef WEB
#include <emscripten/emscripten.h>
#endif

using namespace std;

RenderTexture2D base_tex = (RenderTexture2D){0};
RenderTexture2D main_tex = (RenderTexture2D){0};

void game_loop() {

    // Draw base texture
    // That one is drawn with colors specific for the screen shader to read
    // and generate the main texture.
    BeginTextureMode(base_tex);
    ClearBackground(BLACK);
#ifdef DEV
    string fps = to_string(GetFPS()) + " FPS";
    DrawText(fps.data(), 4, 4, 8, MAIN_MASK_COLOR); // NOLINT
#endif
    game::do_game_loop();
    EndTextureMode();

    // Draw main texture
    // This just renders the base texture with the screen shader.
    BeginTextureMode(main_tex);
    BeginShaderMode(*(shader::get_screen()));
    DrawTexture(base_tex.texture, 0, 0, WHITE);
    EndShaderMode();
    EndTextureMode();

    // Final screen
    // This stretches and centers the main texture on the window, keeping
    // its aspect ratio. It also applies some final post processing.
    auto window_size_x = (float)GetRenderWidth();
    auto window_size_y = (float)GetRenderHeight();
    float aspect_ratio = (float)WINDOW_SIZE_X / (float)WINDOW_SIZE_Y;
    BeginDrawing();
    ClearBackground(BLACK);
    if (window_size_x / window_size_y >= aspect_ratio) {
        float main_tex_x = window_size_y * aspect_ratio;
        DrawTexturePro(main_tex.texture,
                       (Rectangle){0.0f, 0.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y},
                       (Rectangle){(window_size_x - main_tex_x) / 2,
                                   0.0f,
                                   main_tex_x,
                                   window_size_y},
                       (Vector2){0.0f, 0.0f},
                       0.0f,
                       WHITE);
    } else {
        float main_tex_y = window_size_x / aspect_ratio;
        DrawTexturePro(main_tex.texture,
                       (Rectangle){0.0f, 0.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y},
                       (Rectangle){0.0f,
                                   (window_size_y - main_tex_y) / 2,
                                   window_size_x,
                                   main_tex_y},
                       (Vector2){0.0f, 0.0f},
                       0.0f,
                       WHITE);
    }
    EndDrawing();
}

int main() {

#ifndef DEV
    SetTraceLogLevel(LOG_NONE);
#endif

    InitWindow(STARTING_WINDOW_SIZE_X, STARTING_WINDOW_SIZE_Y, WINDOW_TITLE);
    InitAudioDevice();
    SetExitKey(KEY_NULL);

    game::initial_setup();

    // For the sake of the web build, all textures must be powers of 2
    base_tex = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    main_tex = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);

#ifdef WEB
    emscripten_set_main_loop(game_loop, TARGET_FPS, 1);
#else
    SetTargetFPS(TARGET_FPS);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    // To force a size change and update window titlebar buttons
    SetWindowSize(STARTING_WINDOW_SIZE_X, STARTING_WINDOW_SIZE_Y);
    while (!WindowShouldClose()) {
        game_loop();
    }
#endif

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
