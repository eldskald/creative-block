#include "animation.h"
#include "game-element.h"
#include "game.h"
#include "physics-body.h"
#include "sfx.h"
#include "sprite.h"
#include <raylib.h>
#include <raymath.h>
#include <tuple>

using namespace std;

int main() {
    InitWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);
    InitAudioDevice();

    float startup_time = 0.0f;

    game::initial_setup();

    Texture2D atlas = LoadTexture(SPRITESHEET_FILE);
    Shader test_shader = LoadShader(BASE_VERT_SHADER, SHADER_FRAG_1);
    int shader_tint_loc = GetShaderLocation(test_shader, "tint");
    float tint_value[4] = {1.0f, 0.0f, 1.0f, 1.0f};
    SetShaderValue(
        test_shader, shader_tint_loc, tint_value, SHADER_UNIFORM_VEC4);

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
    area->add_child(area_sprite);
    scene->add_child(area);

    tuple<Vector2, float> arr[4] = {
        tuple<Vector2, float>{(Vector2){2, 0}, 1.0f},
        tuple<Vector2, float>{(Vector2){3, 0}, 1.0f},
        tuple<Vector2, float>{(Vector2){2, 1}, 1.0f},
        tuple<Vector2, float>{(Vector2){3, 1}, 1.0f}};
    sprite* animated_sprite = new sprite();
    animated_sprite->pos = (Vector2){380, 210};
    animated_sprite->shader = &test_shader;
    animation<Vector2>* anim =
        new animation<Vector2>(&(animated_sprite->atlas_coords), true, arr, 4);
    animated_sprite->add_child(anim);
    scene->add_child(animated_sprite);

    physics_body* player = new physics_body();
    player->pos = (Vector2){0, 0};
    player->collision_box = (Rectangle){0, 0, 64, 64};
    player->collision_layer = 0b00000001;
    player->collision_mask = 0b00001110;
    sprite* player_sprite = new sprite();
    player_sprite->atlas_coords = (Vector2){0, 1};
    player->add_child(player_sprite);
    scene->add_child(player);
    const float PLAYER_SPEED = 200.0f;

    physics_body* player_child = new physics_body();
    player_child->pos = (Vector2){80, 80};
    player_child->collision_box = (Rectangle){0, 0, 64, 64};
    player_child->collision_layer = 0b00000010;
    player_child->collision_mask = 0b00001101;
    sprite* player_child_sprite = new sprite();
    player_child_sprite->atlas_coords = (Vector2){0, 1};
    player_child->add_child(player_child_sprite);
    player->add_child(player_child);

    physics_body* player_block = new physics_body();
    player_block->pos = (Vector2){72, -72};
    player_block->collision_box = (Rectangle){0, 0, 64, 64};
    player_block->collision_layer = 0b00000010;
    player_block->collision_mask = 0b00000101;
    sprite* player_block_sprite = new sprite();
    player_block_sprite->atlas_coords = (Vector2){0, 0};
    player_block->add_child(player_block_sprite);
    player_child->add_child(player_block);

    sfx* sound_1 = new sfx(sfx::sfx_2);
    scene->add_child(sound_1);

    sfx* sound_2 = new sfx(sfx::sfx_2);
    scene->add_child(sound_2);

    game::set_root(scene);
    anim->play();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(900, 0);

        if (IsKeyPressed(KEY_ONE)) {
            sound_1->play();
        }

        if (IsKeyPressed(KEY_TWO)) {
            sound_2->play();
        }

        Vector2 input_1 =
            (Vector2){(IsKeyDown(KEY_D) ? PLAYER_SPEED : 0.0f) -
                          (IsKeyDown(KEY_A) ? PLAYER_SPEED : 0.0f),
                      (IsKeyDown(KEY_S) ? PLAYER_SPEED : 0.0f) -
                          (IsKeyDown(KEY_W) ? PLAYER_SPEED : 0.0f)};
        player->vel = input_1;

        Vector2 input_2 =
            (Vector2){(IsKeyDown(KEY_L) ? PLAYER_SPEED : 0.0f) -
                          (IsKeyDown(KEY_H) ? PLAYER_SPEED : 0.0f),
                      (IsKeyDown(KEY_J) ? PLAYER_SPEED : 0.0f) -
                          (IsKeyDown(KEY_K) ? PLAYER_SPEED : 0.0f)};
        player_child->vel = input_2;

        game::do_game_loop();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
