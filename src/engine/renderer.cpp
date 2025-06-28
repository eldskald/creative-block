#include "engine/renderer.h"
#include "core/key-gate.h"
#include "defs.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include "engine/text.h"
#include "imports.h"
#include <array>
#include <raylib.h>
#include <string>
#include <vector>

using namespace std;

Texture2D renderer::main_gradient_1_ = (Texture2D){0};
Texture2D renderer::shadow_gradient_1_ = (Texture2D){0};
Texture2D renderer::main_gradient_2_ = (Texture2D){0};
Texture2D renderer::shadow_gradient_2_ = (Texture2D){0};
RenderTexture2D renderer::base_tex_1_ = (RenderTexture2D){0};
RenderTexture2D renderer::base_tex_2_ = (RenderTexture2D){0};
RenderTexture2D renderer::colored_tex_ = (RenderTexture2D){0};
RenderTexture2D renderer::blur_tex_1_ = (RenderTexture2D){0};
RenderTexture2D renderer::blur_tex_2_ = (RenderTexture2D){0};
RenderTexture2D renderer::final_tex_ = (RenderTexture2D){0};
vector<Shader> renderer::screen_color_shaders_;
Shader renderer::base_screen_shader_ = (Shader){0};
Shader renderer::blur_shader_1_ = (Shader){0};
Shader renderer::blur_shader_2_ = (Shader){0};
Vector2 renderer::stretched_tex_size_ = (Vector2){0};
Vector2 renderer::window_size_ = (Vector2){0};
vector<array<float, 4>> renderer::water_waves_;
float renderer::time_ = 0.0f;
int renderer::current_screen_color_shader_ = 0;
#ifdef DEV
bool renderer::showing_areas_ = false;
bool renderer::showing_fixed_bodies_ = false;
bool renderer::showing_kinematic_bodies_ = false;
#endif

const float ASPECT_RATIO = (float)WINDOW_SIZE_X / (float)WINDOW_SIZE_Y;

void renderer::initialize() {
    renderer::main_gradient_1_ = LoadTexture(MAIN_GRADIENT_1_TEXTURE);
    SetTextureFilter(renderer::main_gradient_1_, TEXTURE_FILTER_TRILINEAR);
    renderer::shadow_gradient_1_ = LoadTexture(SHADOW_GRADIENT_1_TEXTURE);
    SetTextureFilter(renderer::shadow_gradient_1_, TEXTURE_FILTER_TRILINEAR);
    renderer::main_gradient_2_ = LoadTexture(MAIN_GRADIENT_2_TEXTURE);
    SetTextureFilter(renderer::main_gradient_2_, TEXTURE_FILTER_TRILINEAR);
    renderer::shadow_gradient_2_ = LoadTexture(SHADOW_GRADIENT_2_TEXTURE);
    SetTextureFilter(renderer::shadow_gradient_2_, TEXTURE_FILTER_TRILINEAR);

    // For the sake of the web build, all textures must be powers of 2
    renderer::base_tex_1_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::base_tex_2_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
    renderer::colored_tex_ = LoadRenderTexture(MAIN_TEX_SIZE, MAIN_TEX_SIZE);
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

    const float normalize_const = 255.0f;
    array<float, 4> mask_main_value = {
        (float)MASK_MAIN_COLOR.r / normalize_const,
        (float)MASK_MAIN_COLOR.g / normalize_const,
        (float)MASK_MAIN_COLOR.b / normalize_const,
        1.0f};
    array<float, 4> mask_shadow_value = {
        (float)MASK_SHADOW_COLOR.r / normalize_const,
        (float)MASK_SHADOW_COLOR.g / normalize_const,
        (float)MASK_SHADOW_COLOR.b / normalize_const,
        1.0f};
    array<float, 4> mask_bg_value = {(float)MASK_BG_COLOR.r / normalize_const,
                                     (float)MASK_BG_COLOR.g / normalize_const,
                                     (float)MASK_BG_COLOR.b / normalize_const,
                                     1.0f};
    float col_bg_factor_value = COL_BG_FACTOR;
    array<float, 4> debug_col_1_value = {
        (float)DEBUG_COLOR_1.r / normalize_const,
        (float)DEBUG_COLOR_1.g / normalize_const,
        (float)DEBUG_COLOR_1.b / normalize_const,
        1.0f};
    array<float, 4> debug_col_2_value = {
        (float)DEBUG_COLOR_2.r / normalize_const,
        (float)DEBUG_COLOR_2.g / normalize_const,
        (float)DEBUG_COLOR_2.b / normalize_const,
        1.0f};
    array<float, 4> debug_col_3_value = {
        (float)DEBUG_COLOR_3.r / normalize_const,
        (float)DEBUG_COLOR_3.g / normalize_const,
        (float)DEBUG_COLOR_3.b / normalize_const,
        1.0f};
    array<float, 4> debug_col_4_value = {
        (float)DEBUG_COLOR_4.r / normalize_const,
        (float)DEBUG_COLOR_4.g / normalize_const,
        (float)DEBUG_COLOR_4.b / normalize_const,
        1.0f};

    // Base screen shader
    renderer::base_screen_shader_ = LoadShader(BASE_VERT_SHADER, SCREEN_SHADER);
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "maskMain",
                                   &mask_main_value,
                                   SHADER_UNIFORM_VEC4);
    array<float, 2> tex_size = {MAIN_TEX_SIZE, MAIN_TEX_SIZE};
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "textureSize",
                                   &tex_size,
                                   SHADER_UNIFORM_VEC2);

    // Screen color shaders
    renderer::screen_color_shaders_.push_back(
        LoadShader(BASE_VERT_SHADER, SCREEN_COLOR_SHADER_0));
    renderer::screen_color_shaders_.push_back(
        LoadShader(BASE_VERT_SHADER, SCREEN_COLOR_SHADER_1));
    renderer::screen_color_shaders_.push_back(
        LoadShader(BASE_VERT_SHADER, SCREEN_COLOR_SHADER_1));
    for (Shader shader : renderer::screen_color_shaders_) {
        renderer::set_shader_property_(
            shader, "maskMain", &mask_main_value, SHADER_UNIFORM_VEC4);
        renderer::set_shader_property_(
            shader, "maskShadow", &mask_shadow_value, SHADER_UNIFORM_VEC4);
        renderer::set_shader_property_(
            shader, "maskBg", &mask_bg_value, SHADER_UNIFORM_VEC4);
        renderer::set_shader_property_(
            shader, "debugCol1", &debug_col_1_value, SHADER_UNIFORM_VEC4);
        renderer::set_shader_property_(
            shader, "debugCol2", &debug_col_2_value, SHADER_UNIFORM_VEC4);
        renderer::set_shader_property_(
            shader, "debugCol3", &debug_col_3_value, SHADER_UNIFORM_VEC4);
        renderer::set_shader_property_(
            shader, "debugCol4", &debug_col_4_value, SHADER_UNIFORM_VEC4);
        renderer::set_shader_property_(shader,
                                       "bgColorFactor",
                                       &col_bg_factor_value,
                                       SHADER_UNIFORM_FLOAT);
    }

    // Two blur shaders for the two passes. The first one blurs with
    // intensity taken into account, and takes the base_tex_1_ so the shader
    // knows where background pixels are to intensify them even more.
    renderer::blur_shader_1_ = LoadShader(BASE_VERT_SHADER, FIRST_BLUR_SHADER);
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
    UnloadTexture(renderer::main_gradient_1_);
    UnloadTexture(renderer::shadow_gradient_1_);
    UnloadTexture(renderer::main_gradient_2_);
    UnloadTexture(renderer::shadow_gradient_2_);
    UnloadRenderTexture(renderer::base_tex_1_);
    UnloadRenderTexture(renderer::base_tex_2_);
    UnloadRenderTexture(renderer::blur_tex_1_);
    UnloadRenderTexture(renderer::blur_tex_2_);
    UnloadRenderTexture(renderer::final_tex_);
    UnloadShader(renderer::base_screen_shader_);
    UnloadShader(renderer::blur_shader_1_);
    UnloadShader(renderer::blur_shader_2_);
    for (Shader shader : renderer::screen_color_shaders_) {
        UnloadShader(shader);
    }
}

void renderer::render() {
    renderer::update_water_waves_();
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
                   (Vector2){0, 0},
                   0.0f,
                   WHITE);
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
#endif
}

void renderer::set_water(bool active, float water_level) {
    auto water = (float)active;
    float level = water_level;
    renderer::set_shader_property_(
        renderer::base_screen_shader_, "water", &water, SHADER_UNIFORM_FLOAT);
    renderer::set_shader_property_(renderer::base_screen_shader_,
                                   "waterLevel",
                                   &level,
                                   SHADER_UNIFORM_FLOAT);
}

void renderer::add_water_wave(float amplitude, float origin, float width) {
    renderer::water_waves_.push_back(
        (array<float, 4>){amplitude, origin, 0.0f, width});
    if (renderer::water_waves_.size() > WATER_MAX_WAVES)
        renderer::water_waves_.erase(renderer::water_waves_.begin());
}

void renderer::reset_water_waves() {
    renderer::water_waves_.clear();
}

void renderer::set_color_shader(int index) {
    renderer::current_screen_color_shader_ = index;
}

void renderer::update_water_waves_() {
    float delta = GetFrameTime();
    for (int i = 0; i < (int)renderer::water_waves_.size(); i++) {
        auto wave = &renderer::water_waves_.at(i);
        wave->at(0) -= delta * WATER_WAVE_DECAY;
        wave->at(2) += delta * WATER_WAVE_SPEED;
        if (wave->at(0) > 0.0f) {
            renderer::set_shader_property_(renderer::base_screen_shader_,
                                           "wave" + to_string(i),
                                           wave,
                                           SHADER_UNIFORM_VEC4);
        } else {
            renderer::water_waves_.erase(renderer::water_waves_.begin() + i);
            i--;
        }
    }
    for (int i = (int)renderer::water_waves_.size(); i < WATER_MAX_WAVES; i++) {
        array<float, 4> zero = {0.0f};
        renderer::set_shader_property_(renderer::base_screen_shader_,
                                       "wave" + to_string(i),
                                       &zero,
                                       SHADER_UNIFORM_VEC4);
    }
}

void renderer::update_color_screen_() {
    renderer::time_ += GetFrameTime();
    renderer::set_shader_property_(renderer::screen_color_shaders_.at(
                                       renderer::current_screen_color_shader_),
                                   "time",
                                   &renderer::time_,
                                   SHADER_UNIFORM_FLOAT);

    if (renderer::current_screen_color_shader_ == 1) {
        int main_gradient_loc = GetShaderLocation(
            renderer::screen_color_shaders_.at(1), "mainGradient");
        SetShaderValueTexture(renderer::screen_color_shaders_.at(1),
                              main_gradient_loc,
                              renderer::main_gradient_1_);
        int shadow_gradient_loc = GetShaderLocation(
            renderer::screen_color_shaders_.at(1), "shadowGradient");
        SetShaderValueTexture(renderer::screen_color_shaders_.at(1),
                              shadow_gradient_loc,
                              renderer::shadow_gradient_1_);
    }

    if (renderer::current_screen_color_shader_ == 2) {
        int main_gradient_loc = GetShaderLocation(
            renderer::screen_color_shaders_.at(2), "mainGradient");
        SetShaderValueTexture(renderer::screen_color_shaders_.at(2),
                              main_gradient_loc,
                              renderer::main_gradient_2_);
        int shadow_gradient_loc = GetShaderLocation(
            renderer::screen_color_shaders_.at(2), "shadowGradient");
        SetShaderValueTexture(renderer::screen_color_shaders_.at(2),
                              shadow_gradient_loc,
                              renderer::shadow_gradient_2_);
    }
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
    key_gate::render_gates_();
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
    BeginTextureMode(renderer::colored_tex_);
    ClearBackground(BLANK);
    BeginShaderMode(renderer::screen_color_shaders_.at(
        renderer::current_screen_color_shader_));
    renderer::update_color_screen_();
    DrawTexturePro(renderer::base_tex_2_.texture,
                   (Rectangle){0.0f, 0.0f, MAIN_TEX_SIZE, -MAIN_TEX_SIZE},
                   (Rectangle){0.0f, 0.0f, MAIN_TEX_SIZE, MAIN_TEX_SIZE},
                   (Vector2){0.0f, 0.0f},
                   0.0f,
                   WHITE);
    EndShaderMode();
    EndTextureMode();

    BeginTextureMode(renderer::blur_tex_1_);
    ClearBackground(BLACK);
    BeginShaderMode(renderer::blur_shader_1_);
    DrawTexturePro(renderer::colored_tex_.texture,
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
    renderer::stretch_texture_(renderer::colored_tex_.texture);
    EndTextureMode();
}

void renderer::set_shader_property_(Shader shader,
                                    string property,
                                    const void* value,
                                    int uniform_type) {
    int uniform_loc = GetShaderLocation(shader, property.c_str());
    SetShaderValue(shader, uniform_loc, value, uniform_type);
}
