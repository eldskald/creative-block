#include "game-element.h"
#include "sprite.h"
#include "game.h"
#include "settings.h"
#include <raylib.h>

int main() {
    InitWindow(game_settings::WINDOW_SIZE_X,
               game_settings::WINDOW_SIZE_Y,
               game_settings::WINDOW_TITLE);
    SetTargetFPS(game_settings::TARGET_FPS);

    game_element* scene = new game_element();
    sprite* img = new sprite();
    scene->add_child(img);
    img->set_texture("assets/textures/face.png");
    img->pos = {200, 256};
    sprite* img2 = new sprite();
    img->add_child(img2);
    img2->set_texture("assets/textures/face.png");
    img2->pos = {72, 128};

    game::set_root(scene);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Hello world!", 190, 200, 20, DARKGREEN);
        EndDrawing();

        game::do_game_loop();
    }

    CloseWindow();

    return 0;
}
