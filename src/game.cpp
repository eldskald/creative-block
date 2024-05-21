#include "game.h"
#include "game-element.h"
#include "inputs.h"
#include "physics-body.h"
#include "sfx.h"
#include "shader.h"
#include "sprite.h"
#include "scene-manager.h"
#include <raylib.h>

game_element* game::root_ = nullptr;

void game::initial_setup() {
    sprite::initialize();
    sfx::initialize();
    shader::initialize();
    scene_manager::initialize();
}

void game::do_game_loop() {
    if (!game::root_) return;
    inputs::tick_();
    physics_body::trigger_physics_tick_(game::root_);
    physics_body::update_areas_();
    game_element::trigger_tick_(game::root_);
    game_element::delete_marked_();
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
