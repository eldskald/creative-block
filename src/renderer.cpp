#include "renderer.h"
#include "sprite.h"
#include <array>
#include <cmath>
#include <raylib.h>
#include <string>

using namespace std;

RenderTexture2D renderer::base_tex_1_ = (RenderTexture2D){0};
RenderTexture2D renderer::base_tex_2_ = (RenderTexture2D){0};
RenderTexture2D renderer::blur_tex_1_ = (RenderTexture2D){0};
RenderTexture2D renderer::blur_tex_2_ = (RenderTexture2D){0};
RenderTexture2D renderer::final_tex_ = (RenderTexture2D){0};
Vector2 renderer::stretched_tex_size_ = (Vector2){0};
Vector2 renderer::window_size_ = (Vector2){0};
Shader renderer::base_screen_shader_ = (Shader){0};
Shader renderer::v_blur_shader_1_ = (Shader){0};
Shader renderer::h_blur_shader_1_ = (Shader){0};
Shader renderer::v_blur_shader_2_ = (Shader){0};
Shader renderer::h_blur_shader_2_ = (Shader){0};

const float aspect_ratio = (float)WINDOW_SIZE_X / (float)WINDOW_SIZE_Y;

void renderer::initialize() {
    // For the sake of the web build, all textures must be powers of 2
    renderer::base_tex_1_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::base_tex_2_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::blur_tex_1_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::blur_tex_2_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    SetTextureFilter(renderer::blur_tex_2_.texture, TEXTURE_FILTER_TRILINEAR);
    renderer::final_tex_ = LoadRenderTexture(2, 2);
    renderer::stretched_tex_size_ = (Vector2){2.0f, 2.0f};
    renderer::window_size_ =
        (Vector2){(float)GetRenderWidth(), (float)GetRenderHeight()};

    // Base screen shader
    renderer::base_screen_shader_ = LoadShader(BASE_VERT_SHADER, SCREEN_SHADER);
    const float normalize_const = 255.0f;
    Color main = MAIN_MASK_COLOR;
    array<float, 4> main_mask_value = {(float)main.r / normalize_const,
                                       (float)main.g / normalize_const,
                                       (float)main.b / normalize_const,
                                       1.0f};
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "mainMask",
                                   &main_mask_value,
                                   SHADER_UNIFORM_VEC4);
    Color shadow = SHADOW_MASK_COLOR;
    array<float, 4> shadow_mask_value = {(float)shadow.r / normalize_const,
                                         (float)shadow.g / normalize_const,
                                         (float)shadow.b / normalize_const,
                                         1.0f};
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "shadowMask",
                                   &shadow_mask_value,
                                   SHADER_UNIFORM_VEC4);
    Color bg = BG_MASK_COLOR;
    array<float, 4> bg_mask_value = {(float)bg.r / normalize_const,
                                     (float)bg.g / normalize_const,
                                     (float)bg.b / normalize_const,
                                     1.0f};
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "bgMask",
                                   &bg_mask_value,
                                   SHADER_UNIFORM_VEC4);

    // Blur shader
    renderer::h_blur_shader_1_ = LoadShader(BASE_VERT_SHADER, BLUR_SHADER);
    renderer::v_blur_shader_1_ = LoadShader(BASE_VERT_SHADER, BLUR_SHADER);
    renderer::h_blur_shader_2_ = LoadShader(BASE_VERT_SHADER, BLUR_SHADER);
    renderer::v_blur_shader_2_ = LoadShader(BASE_VERT_SHADER, BLUR_SHADER);
    array<float, 2> tex_size = {MAIN_TEX_SIZE, MAIN_TEX_SIZE};
    renderer::set_shader_property_(renderer::h_blur_shader_1_,
                                   "textureSize",
                                   &tex_size,
                                   SHADER_UNIFORM_VEC2);
    renderer::set_shader_property_(renderer::v_blur_shader_1_,
                                   "textureSize",
                                   &tex_size,
                                   SHADER_UNIFORM_VEC2);
    renderer::set_shader_property_(renderer::h_blur_shader_2_,
                                   "textureSize",
                                   &tex_size,
                                   SHADER_UNIFORM_VEC2);
    renderer::set_shader_property_(renderer::v_blur_shader_2_,
                                   "textureSize",
                                   &tex_size,
                                   SHADER_UNIFORM_VEC2);
    array<float, 2> right = {1.0f, 0.0f};
    renderer::set_shader_property_(
        renderer::h_blur_shader_1_, "direction", &right, SHADER_UNIFORM_VEC2);
    renderer::set_shader_property_(
        renderer::h_blur_shader_2_, "direction", &right, SHADER_UNIFORM_VEC2);
    array<float, 2> up = {0.0f, -1.0f};
    renderer::set_shader_property_(
        renderer::v_blur_shader_1_, "direction", &up, SHADER_UNIFORM_VEC2);
    renderer::set_shader_property_(
        renderer::v_blur_shader_2_, "direction", &up, SHADER_UNIFORM_VEC2);
}

void renderer::render() {
    renderer::update_window_size_();
    renderer::update_tex_sizes_();
    renderer::render_base_();
    renderer::post_process_();
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(renderer::final_tex_.texture,
                   (Rectangle){0.0f,
                               0.0f,
                               renderer::stretched_tex_size_.x,
                               -renderer::stretched_tex_size_.y},
                   (Rectangle){0.0f,
                               0.0f,
                               renderer::stretched_tex_size_.x,
                               renderer::stretched_tex_size_.y},
                   (Vector2){0.0f, 0.0f},
                   0.0f,
                   WHITE);
    EndDrawing();
}

void renderer::update_window_size_() {
    renderer::window_size_.x = (float)GetRenderWidth();
    renderer::window_size_.y = (float)GetRenderHeight();
}

void renderer::update_tex_sizes_() {
    float window_size_x = (float)GetRenderWidth();
    float window_size_y = (float)GetRenderHeight();
    bool increased_size = false;

    // Multiplying sizes by 2 so they remain powers of 2
    while (renderer::stretched_tex_size_.x < window_size_x) {
        increased_size = true;
        renderer::stretched_tex_size_.x *= 2;
    }
    while (renderer::stretched_tex_size_.y < window_size_y) {
        increased_size = true;
        renderer::stretched_tex_size_.y *= 2;
    }

    if (increased_size) {
        UnloadRenderTexture(renderer::final_tex_);
        renderer::final_tex_ =
            LoadRenderTexture((int)renderer::stretched_tex_size_.x,
                              (int)renderer::stretched_tex_size_.y);
    }
}

// Base texture is the base low res pixel art game texture, with no
// stretching to fill the window or post-processin.
void renderer::render_base_() {
    BeginTextureMode(renderer::base_tex_1_);
    ClearBackground(BLANK);
#ifdef DEV
    string fps = to_string(GetFPS()) + " FPS";
    DrawText(fps.data(), 4, 4, 8, MAIN_MASK_COLOR); // NOLINT
#endif
    sprite::render_sprites_();
    EndTextureMode();

    BeginTextureMode(renderer::base_tex_2_);
    ClearBackground(BLANK);
    BeginShaderMode(renderer::base_screen_shader_);
    DrawTexture(renderer::base_tex_1_.texture, 0, 0, WHITE);
    EndShaderMode();
    EndTextureMode();
}

// Takes a texture of size WINDOW_SIZE_X by WINDOW_SIZE_Y and stretches it to
// fill the window size while maintaining aspect ratio and centralized.
void renderer::stretch_texture_(Texture2D texture) {
    if (renderer::window_size_.x / renderer::window_size_.y >= aspect_ratio) {
        float stretched_tex_x = renderer::window_size_.y * aspect_ratio;
        DrawTexturePro(
            texture,
            (Rectangle){0.0f, 0.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y},
            (Rectangle){(renderer::window_size_.x - stretched_tex_x) / 2,
                        0.0f,
                        stretched_tex_x,
                        renderer::window_size_.y},
            (Vector2){0.0f, 0.0f},
            0.0f,
            WHITE);
    } else {
        float stretched_tex_y = renderer::window_size_.x / aspect_ratio;
        DrawTexturePro(
            texture,
            (Rectangle){0.0f, 0.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y},
            (Rectangle){0.0f,
                        (renderer::window_size_.y - stretched_tex_y) / 2,
                        renderer::window_size_.x,
                        stretched_tex_y},
            (Vector2){0.0f, 0.0f},
            0.0f,
            WHITE);
    }
}

void renderer::post_process_() {
    BeginTextureMode(renderer::blur_tex_1_);
    ClearBackground(BLACK);
    BeginShaderMode(renderer::h_blur_shader_1_);
    DrawTexture(renderer::base_tex_2_.texture, 0, 0, WHITE);
    EndShaderMode();
    EndTextureMode();

    BeginTextureMode(renderer::blur_tex_2_);
    ClearBackground(BLACK);
    BeginShaderMode(renderer::v_blur_shader_1_);
    DrawTexture(renderer::blur_tex_1_.texture, 0, 0, WHITE);
    EndShaderMode();
    EndTextureMode();

    BeginTextureMode(renderer::final_tex_);
    ClearBackground(BLACK);
    renderer::stretch_texture_(renderer::blur_tex_2_.texture);
    renderer::stretch_texture_(renderer::base_tex_2_.texture);
    EndTextureMode();
}

void renderer::set_shader_property_(Shader shader,
                                    string property,
                                    const void* value,
                                    int uniform_type) {
    int uniform_loc = GetShaderLocation(shader, property.c_str());
    SetShaderValue(shader, uniform_loc, value, uniform_type);
}
