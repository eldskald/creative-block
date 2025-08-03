#include "core/game.h"
#include "core/scene-manager.h"
#include "engine/bgm.h"
#include "engine/game-element.h"
#include "engine/inputs.h"
#include "engine/physics-body.h"
#include "engine/renderer.h"
#include "engine/sfx.h"
#include "engine/sprite.h"
#include "engine/text.h"
#include <raylib.h>

game_element* game::root_ = nullptr;
bool game::paused_ = false;

void game::on_start() {
    SetRandomSeed(0);
    renderer::initialize();
    sprite::initialize();
    sfx::initialize();
    bgm::initialize();
    text::initialize();
    scene_manager::initialize();
    inputs::initialize();
}

void game::on_close() {
    renderer::unload();
    sprite::unload();
    sfx::unload();
    bgm::unload();
    text::unload();
}

void game::do_game_loop() {
    if (!game::root_) return;
    inputs::tick_();
    if (!game::paused_) {
        physics_body::trigger_physics_tick_(game::root_);
        physics_body::update_areas_();
        game_element::trigger_tick_(game::root_);
        bgm::tick_();
    }
    sfx::tick_();
    renderer::render();
    game_element::reparent_elements_();
    game_element::delete_marked_();
    scene_manager::tick_();
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

bool game::is_paused() {
    return game::paused_;
}

void game::toggle_pause() {
    game::paused_ = !game::paused_;
}
