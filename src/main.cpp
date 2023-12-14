#include "animation.h"
#include "game-element.h"
#include "game.h"
#include "physics-body.h"
#include "sfx.h"
#include "sprite.h"
#include <raylib.h>
#include <tuple>

using namespace std;

int main() {
    InitWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);
    InitAudioDevice();

    game::initial_setup();

    game_element* scene = new game_element();

    physics_body* block_1 = new physics_body();
    block_1->pos = (Vector2){120, 290};
    block_1->collision_box = (Rectangle){0, 0, 64, 64};
    block_1->static_body = true;
    sprite* sprite_1 = new sprite();
    sprite_1->atlas_coords = (Vector2){0, 0};
    block_1->add_child(sprite_1);
    scene->add_child(block_1);

    physics_body* block_2 = new physics_body();
    block_2->pos = (Vector2){150, 250};
    block_2->collision_box = (Rectangle){0, 0, 64, 64};
    block_2->static_body = true;
    sprite* sprite_2 = new sprite();
    sprite_2->atlas_coords = (Vector2){0, 0};
    block_2->add_child(sprite_2);
    scene->add_child(block_2);

    tuple<Vector2, float> arr[4] = {
        tuple<Vector2, float>{(Vector2){2, 0}, 1.0f},
        tuple<Vector2, float>{(Vector2){3, 0}, 1.0f},
        tuple<Vector2, float>{(Vector2){2, 1}, 1.0f},
        tuple<Vector2, float>{(Vector2){3, 1}, 1.0f}};
    sprite* animated_sprite = new sprite();
    animated_sprite->pos = (Vector2){380, 210};
    animation<Vector2>* anim =
        new animation<Vector2>(&(animated_sprite->atlas_coords), true, arr, 4);
    animated_sprite->add_child(anim);
    scene->add_child(animated_sprite);

    physics_body* player = new physics_body();
    player->pos = (Vector2){0, 0};
    player->collision_box = (Rectangle){0, 0, 64, 64};
    sprite* player_sprite = new sprite();
    player_sprite->atlas_coords = (Vector2){0, 1};
    player->add_child(player_sprite);
    scene->add_child(player);
    const float PLAYER_SPEED = 200.0f;

    sfx* sound_1 = new sfx(sfx::sfx_2);
    scene->add_child(sound_1);

    sfx* sound_2 = new sfx(sfx::sfx_2);
    scene->add_child(sound_2);

    game::set_root(scene);
    anim->play();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();

        if (IsKeyPressed(KEY_ONE)) {
            sound_1->play();
        }

        if (IsKeyPressed(KEY_TWO)) {
            sound_2->play();
        }

        Vector2 input = (Vector2){(IsKeyDown(KEY_D) ? PLAYER_SPEED : 0.0f) -
                                      (IsKeyDown(KEY_A) ? PLAYER_SPEED : 0.0f),
                                  (IsKeyDown(KEY_S) ? PLAYER_SPEED : 0.0f) -
                                      (IsKeyDown(KEY_W) ? PLAYER_SPEED : 0.0f)};
        player->vel = input;

        game::do_game_loop();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
