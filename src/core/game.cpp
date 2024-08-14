#include "core/game.h"
#include "engine/game-element.h"
#include "engine/inputs.h"
#include "engine/physics-body.h"
#include "core/scene-manager.h"
#include "engine/sfx.h"
#include "engine/sprite.h"
#include "engine/renderer.h"
#include <raylib.h>

game_element* game::root_ = nullptr;

void game::initial_setup() {
    SetRandomSeed(0);
    renderer::initialize();
    sprite::initialize();
    sfx::initialize();
    scene_manager::initialize();
}

void game::do_game_loop() {
    if (!game::root_) return;
    inputs::tick_();
    physics_body::trigger_physics_tick_(game::root_);
    physics_body::update_areas_();
    game_element::trigger_tick_(game::root_);
    renderer::render();
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
