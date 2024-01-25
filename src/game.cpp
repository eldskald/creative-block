#include "game.h"
#include "game-element.h"
#include "physics-body.h"
#include "sfx.h"
#include "sprite.h"
#include <raylib.h>

game_element* game::root_ = nullptr;

void game::initial_setup() {
    sprite::atlas_ = LoadTexture(SPRITESHEET_FILE);
    *(sprite::base_shader) = LoadShader(BASE_VERT_SHADER, BASE_FRAG_SHADER);
    sfx::sfx_1 = LoadWave(SFX_1);
    sfx::sfx_2 = LoadWave(SFX_2);
}

void game::do_game_loop() {
    if (!game::root_) return;
    physics_body::trigger_physics_tick_(game::root_);
    physics_body::update_areas_();
    game_element::trigger_tick_(game::root_);
}

void game::set_root(game_element* new_root) {
    if (game::root_) {
        game::root_->trigger_exit_(game::root_);
    }
    game::root_ = new_root;
    game_element::trigger_enter_(new_root);
}

game_element* game::get_root() {
    return game::root_;
}
