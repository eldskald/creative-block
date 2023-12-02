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

    game::initial_setup();

    game_element* scene = new game_element();
    sprite* img = new sprite();
    scene->add_child(img);
    img->set_texture(2, 0);
    img->pos = (Vector2){200, 100};

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
