#include "animation.h"
#include "game-element.h"
#include "game.h"
#include "particle-effect.h"
#include "physics-body.h"
#include "sfx.h"
#include "shader.h"
#include "sprite.h"
#include <raylib.h>
#include <raymath.h>
#include <tuple>
#include <vector>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

using namespace std;

RenderTexture2D main_tex = (RenderTexture2D){0};
sfx* sound_1 = nullptr;
sfx* sound_2 = nullptr;
particle_effect* emitter = nullptr;
physics_body* player = nullptr;

void game_loop() {
    BeginTextureMode(main_tex);
    ClearBackground(BLACK);
    DrawFPS(900, 0); // NOLINT

    if (IsKeyPressed(KEY_ONE)) {
        sound_1->play();
    }

    if (IsKeyPressed(KEY_TWO)) {
        sound_2->play();
    }

    if (IsKeyPressed(KEY_SPACE)) {
        emitter->emit();
    }

    Vector2 input_1 = (Vector2){(IsKeyDown(KEY_D) ? PLAYER_SPEED : 0.0f) -
                                    (IsKeyDown(KEY_A) ? PLAYER_SPEED : 0.0f),
                                (IsKeyDown(KEY_S) ? PLAYER_SPEED : 0.0f) -
                                    (IsKeyDown(KEY_W) ? PLAYER_SPEED : 0.0f)};
    player->vel = input_1;

    game::do_game_loop();
    EndTextureMode();

    auto window_size_x = (float)GetScreenWidth();
    auto window_size_y = (float)GetScreenHeight();
    float aspect_ratio = (float)WINDOW_SIZE_X / (float)WINDOW_SIZE_Y;
    BeginDrawing();
    ClearBackground(BLACK);
    if (window_size_x / window_size_y >= aspect_ratio) {
        float main_tex_x = window_size_y * aspect_ratio;
        DrawTexturePro(
            main_tex.texture,
            (Rectangle){0.0f, WINDOW_SIZE_Y, WINDOW_SIZE_X, -WINDOW_SIZE_Y},
            (Rectangle){(window_size_x - main_tex_x) / 2,
                        0.0f,
                        main_tex_x,
                        window_size_y},
            (Vector2){0.0f, 0.0f},
            0.0f,
            WHITE);
    } else {
        float main_tex_y = window_size_x / aspect_ratio;
        DrawTexturePro(
            main_tex.texture,
            (Rectangle){0.0f, WINDOW_SIZE_Y, WINDOW_SIZE_X, -WINDOW_SIZE_Y},
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

    // #if !defined(DEV)
    //     SetTraceLogLevel(LOG_NONE);
    // #endif

    InitWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, WINDOW_TITLE);
    InitAudioDevice();

    game::initial_setup();

    // NOLINTBEGIN
    game_element* scene = new game_element();

    physics_body* block_1 = new physics_body();
    block_1->pos = (Vector2){120, 290};
    block_1->collision_box = (Rectangle){0, 0, 64, 64};
    block_1->type = physics_body::fixed;
    block_1->collision_layer = 0b00000100;
    sprite* sprite_1 = new sprite();
    sprite_1->atlas_coords = (Vector2){0, 0};
    block_1->add_child(sprite_1);
    scene->add_child(block_1);

    physics_body* block_2 = new physics_body();
    block_2->pos = (Vector2){150, 250};
    block_2->collision_box = (Rectangle){0, 0, 64, 64};
    block_2->type = physics_body::fixed;
    block_2->collision_layer = 0b00000100;
    sprite* sprite_2 = new sprite();
    sprite_2->atlas_coords = (Vector2){0, 0};
    block_2->add_child(sprite_2);
    scene->add_child(block_2);

    physics_body* one_way_block = new physics_body();
    one_way_block->pos = (Vector2){280, 400};
    one_way_block->collision_box = (Rectangle){0, 0, 64, 64};
    one_way_block->type = physics_body::fixed;
    one_way_block->one_way = true;
    one_way_block->collision_layer = 0b00001000;
    sprite* one_way_sprite = new sprite();
    one_way_sprite->atlas_coords = (Vector2){0, 0};
    one_way_block->add_child(one_way_sprite);
    scene->add_child(one_way_block);

    physics_body* area = new physics_body();
    area->pos = (Vector2){800, 300};
    area->collision_box = (Rectangle){0, 0, 64, 64};
    area->type = physics_body::area;
    area->collision_mask = 0b00000001;
    sprite* area_sprite = new sprite();
    area_sprite->atlas_coords = (Vector2){0, 0};
    area_sprite->tint = RED;
    area->add_child(area_sprite);
    scene->add_child(area);

    vector<tuple<Vector2, float>> frames =
        vector({tuple<Vector2, float>{(Vector2){2, 0}, 1.0f},
                tuple<Vector2, float>{(Vector2){3, 0}, 1.0f},
                tuple<Vector2, float>{(Vector2){2, 1}, 1.0f},
                tuple<Vector2, float>{(Vector2){3, 1}, 1.0f}});
    sprite* animated_sprite = new sprite();
    animated_sprite->pos = (Vector2){380, 210};
    animated_sprite->shader = shader::get_sample();
    animation<Vector2>* anim =
        new animation<Vector2>(&(animated_sprite->atlas_coords), true, frames);
    animated_sprite->add_child(anim);
    scene->add_child(animated_sprite);

    player = new physics_body();
    player->pos = (Vector2){0, 0};
    player->collision_box = (Rectangle){0, 0, 64, 64};
    player->collision_layer = 0b00000001;
    player->collision_mask = 0b00001110;
    sprite* player_sprite = new sprite();
    player_sprite->atlas_coords = (Vector2){0, 1};
    player->add_child(player_sprite);
    scene->add_child(player);

    emitter = new particle_effect();
    emitter->particles = 8;
    emitter->lifetime = 6.0f;
    emitter->lifetime_variation = 0.5f;
    emitter->mask_atlas_coords = (Vector2){0, 1};
    emitter->mask_tint = WHITE;
    emitter->mask_shader = *(shader::get_particle());
    emitter->speed = 200.0f;
    emitter->speed_variation = 0.5f;
    emitter->acceleration = (Vector2){0, 400};
    emitter->spawn_area = (Rectangle){0, 0, 1, 1};
    emitter->direction_angle = 0.0f;
    emitter->spread_angle = PI;
    emitter->particle_collision_box = (Rectangle){0, 0, 64, 64};
    emitter->elasticity_factor = 0.5f;
    player->add_child(emitter);

    sound_1 = new sfx(sfx::sfx_2);
    scene->add_child(sound_1);

    sound_2 = new sfx(sfx::sfx_2);
    scene->add_child(sound_2);

    game::set_root(scene);
    anim->play();
    // NOLINTEND

    main_tex = LoadRenderTexture(WINDOW_SIZE_X, WINDOW_SIZE_Y);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(game_loop, TARGET_FPS, 1);
#else
    SetTargetFPS(TARGET_FPS);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    // To force a size change and update window titlebar buttons
    SetWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    while (!WindowShouldClose()) {
        game_loop();
    }
#endif

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
