#include "engine/renderer.h"
#include "defs.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "engine/text.h"
#include "imports.h"
#include <array>
#include <raylib.h>
#include <string>

using namespace std;

RenderTexture2D renderer::base_tex_1_ = (RenderTexture2D){0};
RenderTexture2D renderer::base_tex_2_ = (RenderTexture2D){0};
RenderTexture2D renderer::blur_tex_1_ = (RenderTexture2D){0};
RenderTexture2D renderer::blur_tex_2_ = (RenderTexture2D){0};
RenderTexture2D renderer::final_tex_ = (RenderTexture2D){0};
Shader renderer::base_screen_shader_ = (Shader){0};
Shader renderer::blur_shader_1_ = (Shader){0};
Shader renderer::blur_shader_2_ = (Shader){0};
Vector2 renderer::stretched_tex_size_ = (Vector2){0};
Vector2 renderer::window_size_ = (Vector2){0};
#ifdef DEV
bool renderer::showing_areas_ = false;
bool renderer::showing_fixed_bodies_ = false;
bool renderer::showing_kinematic_bodies_ = false;
renderer::bloom_debug renderer::texture_rendered_ = final;
#endif

const float ASPECT_RATIO = (float)WINDOW_SIZE_X / (float)WINDOW_SIZE_Y;

void renderer::initialize() {
    // For the sake of the web build, all textures must be powers of 2
    renderer::base_tex_1_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::base_tex_2_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::blur_tex_1_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    SetTextureFilter(renderer::blur_tex_1_.texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureWrap(renderer::blur_tex_1_.texture, TEXTURE_WRAP_CLAMP);
    renderer::blur_tex_2_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    SetTextureFilter(renderer::blur_tex_2_.texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureWrap(renderer::blur_tex_2_.texture, TEXTURE_WRAP_CLAMP);
    renderer::final_tex_ = LoadRenderTexture(2, 2);
    renderer::final_tex_ = LoadRenderTexture(2, 2);
    renderer::stretched_tex_size_ = (Vector2){2, 2};
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
    Color debug_1 = DEBUG_COLOR_1;
    array<float, 4> debug_1_value = {(float)debug_1.r / normalize_const,
                                     (float)debug_1.g / normalize_const,
                                     (float)debug_1.b / normalize_const,
                                     1.0f};
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "debug1",
                                   &debug_1_value,
                                   SHADER_UNIFORM_VEC4);
    Color debug_2 = DEBUG_COLOR_2;
    array<float, 4> debug_2_value = {(float)debug_2.r / normalize_const,
                                     (float)debug_2.g / normalize_const,
                                     (float)debug_2.b / normalize_const,
                                     1.0f};
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "debug2",
                                   &debug_2_value,
                                   SHADER_UNIFORM_VEC4);
    Color debug_3 = DEBUG_COLOR_3;
    array<float, 4> debug_3_value = {(float)debug_3.r / normalize_const,
                                     (float)debug_3.g / normalize_const,
                                     (float)debug_3.b / normalize_const,
                                     1.0f};
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "debug3",
                                   &debug_3_value,
                                   SHADER_UNIFORM_VEC4);
    Color debug_4 = DEBUG_COLOR_4;
    array<float, 4> debug_4_value = {(float)debug_4.r / normalize_const,
                                     (float)debug_4.g / normalize_const,
                                     (float)debug_4.b / normalize_const,
                                     1.0f};
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "debug4",
                                   &debug_4_value,
                                   SHADER_UNIFORM_VEC4);

    // Two blur shaders for the two passes. The first one blurs with intensity
    // taken into account, and takes the base_tex_1_ so the shader knows where
    // background pixels are to intensify them even more.
    renderer::blur_shader_1_ = LoadShader(BASE_VERT_SHADER, FIRST_BLUR_SHADER);
    array<float, 2> tex_size = {MAIN_TEX_SIZE, MAIN_TEX_SIZE};
    renderer::set_shader_property_(renderer::blur_shader_1_,
                                   "textureSize",
                                   &tex_size,
                                   SHADER_UNIFORM_VEC2);
    renderer::blur_shader_2_ = LoadShader(BASE_VERT_SHADER, SECOND_BLUR_SHADER);
    renderer::set_shader_property_(renderer::blur_shader_2_,
                                   "textureSize",
                                   &tex_size,
                                   SHADER_UNIFORM_VEC2);
}

void renderer::unload() {
    UnloadRenderTexture(renderer::base_tex_1_);
    UnloadRenderTexture(renderer::base_tex_2_);
    UnloadRenderTexture(renderer::blur_tex_1_);
    UnloadRenderTexture(renderer::blur_tex_2_);
    UnloadRenderTexture(renderer::final_tex_);
    UnloadShader(renderer::base_screen_shader_);
    UnloadShader(renderer::blur_shader_1_);
    UnloadShader(renderer::blur_shader_2_);
}

void renderer::render() {
    renderer::update_window_size_();
    renderer::update_tex_sizes_();
    renderer::render_base_();
    renderer::post_process_();
    BeginDrawing();
    ClearBackground(BLACK);
#ifdef DEV
    switch (renderer::texture_rendered_) {
    case final: {
        DrawTexturePro(renderer::final_tex_.texture,
                       (Rectangle){0.0f,
                                   0.0f,
                                   renderer::stretched_tex_size_.x,
                                   -renderer::stretched_tex_size_.y},
                       (Rectangle){0.0f,
                                   0.0f,
                                   renderer::stretched_tex_size_.x,
                                   renderer::stretched_tex_size_.y},
                       (Vector2){0, 0},
                       0.0f,
                       WHITE);
        break;
    }
    case base_1: {
        renderer::stretch_texture_(base_tex_1_.texture);
        break;
    }
    case base_2: {
        renderer::stretch_texture_(base_tex_2_.texture);
        break;
    }
    case blur_1: {
        renderer::stretch_texture_(blur_tex_1_.texture);
        break;
    }
    case blur_2: {
        renderer::stretch_texture_(blur_tex_2_.texture);
        break;
    }
    }
#else
    DrawTexturePro(renderer::final_tex_.texture,
                   (Rectangle){0.0f,
                               0.0f,
                               renderer::stretched_tex_size_.x,
                               -renderer::stretched_tex_size_.y},
                   (Rectangle){0.0f,
                               0.0f,
                               renderer::stretched_tex_size_.x,
                               renderer::stretched_tex_size_.y},
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
#endif
    EndDrawing();
#ifdef DEV
    if (IsKeyPressed(DEBUG_CTL_SHOW_AREAS)) {
        renderer::showing_areas_ = !renderer::showing_areas_;
    }
    if (IsKeyPressed(DEBUG_CTL_SHOW_FIXED_BODIES)) {
        renderer::showing_fixed_bodies_ = !renderer::showing_fixed_bodies_;
    }
    if (IsKeyPressed(DEBUG_CTL_SHOW_KINEMATIC_BODIES)) {
        renderer::showing_kinematic_bodies_ =
            !renderer::showing_kinematic_bodies_;
    }
    if (IsKeyPressed(DEBUG_CTL_RENDER_FINAL)) {
        renderer::texture_rendered_ = final;
    }
    if (IsKeyPressed(DEBUG_CTL_RENDER_BLUR_1)) {
        renderer::texture_rendered_ = blur_1;
    }
    if (IsKeyPressed(DEBUG_CTL_RENDER_BLUR_2)) {
        renderer::texture_rendered_ = blur_2;
    }
    if (IsKeyPressed(DEBUG_CTL_RENDER_BASE_1)) {
        renderer::texture_rendered_ = base_1;
    }
    if (IsKeyPressed(DEBUG_CTL_RENDER_BASE_2)) {
        renderer::texture_rendered_ = base_2;
    }
#endif
}

void renderer::update_window_size_() {
    renderer::window_size_.x = (float)GetRenderWidth();
    renderer::window_size_.y = (float)GetRenderHeight();
}

void renderer::update_tex_sizes_() {
    auto window_size_x = (float)GetRenderWidth();
    auto window_size_y = (float)GetRenderHeight();
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
    sprite::render_sprites_();
    text::render_texts_();
#ifdef DEV
    string fps = to_string(GetFPS()) + " FPS";
    DrawText(fps.data(), 4, 4, 8, DEBUG_COLOR_1); // NOLINT
    if (renderer::showing_areas_) {
        for (auto* area : physics_body::areas_) {
            DrawRectangleLines(
                (int)area->get_global_pos().x + (int)area->collision_box.x,
                (int)area->get_global_pos().y + (int)area->collision_box.y,
                (int)area->collision_box.width,
                (int)area->collision_box.height,
                DEBUG_COLOR_2);
        }
    }
    if (renderer::showing_fixed_bodies_) {
        for (auto* body : physics_body::bodies_) {
            if (body->type != physics_body::body_type::fixed) continue;
            DrawRectangleLines(
                (int)body->get_global_pos().x + (int)body->collision_box.x,
                (int)body->get_global_pos().y + (int)body->collision_box.y,
                (int)body->collision_box.width,
                (int)body->collision_box.height,
                DEBUG_COLOR_3);
        }
    }
    if (renderer::showing_kinematic_bodies_) {
        for (auto* body : physics_body::bodies_) {
            if (body->type != physics_body::body_type::kinematic) continue;
            DrawRectangleLines(
                (int)body->get_global_pos().x + (int)body->collision_box.x,
                (int)body->get_global_pos().y + (int)body->collision_box.y,
                (int)body->collision_box.width,
                (int)body->collision_box.height,
                DEBUG_COLOR_4);
        }
    }
#endif
    EndTextureMode();

    BeginTextureMode(renderer::base_tex_2_);
    ClearBackground(BLANK);
    BeginShaderMode(renderer::base_screen_shader_);
    DrawTexturePro(renderer::base_tex_1_.texture,
                   (Rectangle){0, 0, MAIN_TEX_SIZE, -MAIN_TEX_SIZE},
                   (Rectangle){0, 0, MAIN_TEX_SIZE, MAIN_TEX_SIZE},
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
    EndShaderMode();
    EndTextureMode();
}

// Takes a texture of size WINDOW_SIZE_X by WINDOW_SIZE_Y and stretches it to
// fill the window size while maintaining aspect ratio and centralized, also it
// assumes the texture is a RenderTexture and thus y-inverted.
void renderer::stretch_texture_(Texture2D texture) {
    if (renderer::window_size_.x / renderer::window_size_.y >= ASPECT_RATIO) {
        float stretched_tex_x = renderer::window_size_.y * ASPECT_RATIO;
        DrawTexturePro(
            texture,
            (Rectangle){0.0f,
                        MAIN_TEX_SIZE - WINDOW_SIZE_Y,
                        WINDOW_SIZE_X,
                        -WINDOW_SIZE_Y},
            (Rectangle){(renderer::window_size_.x - stretched_tex_x) / 2,
                        0.0f,
                        stretched_tex_x,
                        renderer::window_size_.y},
            (Vector2){0.0f, 0.0f},
            0.0f,
            WHITE);
    } else {
        float stretched_tex_y = renderer::window_size_.x / ASPECT_RATIO;
        DrawTexturePro(
            texture,
            (Rectangle){0.0f,
                        MAIN_TEX_SIZE - WINDOW_SIZE_Y,
                        WINDOW_SIZE_X,
                        -WINDOW_SIZE_Y},
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
    BeginShaderMode(renderer::blur_shader_1_);
    DrawTexturePro(renderer::base_tex_2_.texture,
                   (Rectangle){0.0f, 0.0f, MAIN_TEX_SIZE, -MAIN_TEX_SIZE},
                   (Rectangle){0.0f, 0.0f, MAIN_TEX_SIZE, MAIN_TEX_SIZE},
                   (Vector2){0.0f, 0.0f},
                   0.0f,
                   WHITE);
    EndShaderMode();
    EndTextureMode();

    BeginTextureMode(renderer::blur_tex_2_);
    ClearBackground(BLACK);
    BeginShaderMode(renderer::blur_shader_2_);
    DrawTexturePro(renderer::blur_tex_1_.texture,
                   (Rectangle){0.0f, 0.0f, MAIN_TEX_SIZE, -MAIN_TEX_SIZE},
                   (Rectangle){0.0f, 0.0f, MAIN_TEX_SIZE, MAIN_TEX_SIZE},
                   (Vector2){0.0f, 0.0f},
                   0.0f,
                   WHITE);
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
