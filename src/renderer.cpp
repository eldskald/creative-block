#include "renderer.h"
#include "sprite.h"
#include <raylib.h>
#include <string>

using namespace std;

RenderTexture2D renderer::base_tex_step_1_ = (RenderTexture2D){0};
RenderTexture2D renderer::base_tex_ = (RenderTexture2D){0};
RenderTexture2D renderer::stretched_tex_ = (RenderTexture2D){0};

void renderer::initialize() {
    // For the sake of the web build, all textures must be powers of 2
    renderer::base_tex_step_1_ =
        LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::base_tex_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::stretched_tex_ =
        LoadRenderTexture(MAX_RESOLUTION, MAX_RESOLUTION);
}

void renderer::render() {
    renderer::render_base_();
    renderer::render_stretched_();
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(renderer::stretched_tex_.texture,
                   (Rectangle){0, 0, MAX_RESOLUTION, -MAX_RESOLUTION},
                   (Rectangle){0, 0, MAX_RESOLUTION, MAX_RESOLUTION},
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
    EndDrawing();
}

// Base texture is the base low res pixel art game texture, with no
// stretching to fill the window or post-processin.
void renderer::render_base_() {
    BeginTextureMode(base_tex_step_1_);
    ClearBackground(BLACK);
#ifdef DEV
    string fps = to_string(GetFPS()) + " FPS";
    DrawText(fps.data(), 4, 4, 8, MAIN_MASK_COLOR); // NOLINT
#endif
    sprite::render_sprites_();
    EndTextureMode();

    BeginTextureMode(base_tex_);
    BeginShaderMode(*(shader::get_screen()));
    DrawTexture(base_tex_step_1_.texture, 0, 0, WHITE);
    EndShaderMode();
    EndTextureMode();
}

// Stretched texture is the base texture stretched to fill the screen space.
void renderer::render_stretched_() {
    auto window_size_x = (float)GetRenderWidth();
    auto window_size_y = (float)GetRenderHeight();
    float aspect_ratio = (float)WINDOW_SIZE_X / (float)WINDOW_SIZE_Y;

    BeginTextureMode(stretched_tex_);
    ClearBackground(BLACK);
    if (window_size_x / window_size_y >= aspect_ratio) {
        float stretched_tex_x = window_size_y * aspect_ratio;
        DrawTexturePro(base_tex_.texture,
                       (Rectangle){0.0f, 0.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y},
                       (Rectangle){(window_size_x - stretched_tex_x) / 2,
                                   0.0f,
                                   stretched_tex_x,
                                   window_size_y},
                       (Vector2){0.0f, 0.0f},
                       0.0f,
                       WHITE);
    } else {
        float stretched_tex_y = window_size_x / aspect_ratio;
        DrawTexturePro(base_tex_.texture,
                       (Rectangle){0.0f, 0.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y},
                       (Rectangle){0.0f,
                                   (window_size_y - stretched_tex_y) / 2,
                                   window_size_x,
                                   stretched_tex_y},
                       (Vector2){0.0f, 0.0f},
                       0.0f,
                       WHITE);
    }
    EndTextureMode();
}
