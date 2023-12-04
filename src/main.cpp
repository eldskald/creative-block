#include "defs.h"
#include "game-element.h"
#include "game.h"
#include "settings.h"
#include "sprite.h"
#include <raylib.h>
#include <vector>

using namespace std;

int main() {
    InitWindow(game_settings::WINDOW_SIZE_X,
               game_settings::WINDOW_SIZE_Y,
               game_settings::WINDOW_TITLE);
    SetTargetFPS(game_settings::TARGET_FPS);

    game::initial_setup();

    game_element* scene = new game_element();
    sprite* img = new sprite();
    scene->add_child(img);
    img->set_animation(vector<defs::animation_frame>{
        (defs::animation_frame){1, 2, 0.5f},
        (defs::animation_frame){1, 0, 0.5f},
        (defs::animation_frame){0, 1, 0.5f},
        (defs::animation_frame){1, 1, 0.5f}
    });
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
