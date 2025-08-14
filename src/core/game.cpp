#include "core/game.h"
#include "core/pause-menu.h"
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

#if !WEB && !NOFULLSCREEN
bool game::first_frame_ = false;
bool game::second_frame_ = false;
#endif

#ifndef WEB
bool game::game_closing_ = false;

void game::close_game() {
    game::game_closing_ = true;
}

bool game::is_game_closing() {
    return WindowShouldClose() || game::game_closing_;
}
#endif

void game::on_start() {
    SetRandomSeed(0);
    renderer::initialize();
    sprite::initialize();
    sfx::initialize();
    bgm::initialize();
    text::initialize();
    scene_manager::initialize();
    inputs::initialize();
#ifdef MUTE
    SetMasterVolume(0.0f);
#endif
}

void game::on_close() {
    renderer::unload();
    sprite::unload();
    sfx::unload();
    bgm::unload();
    text::unload();
}

void game::do_game_loop() {
#if !WEB && !NOFULLSCREEN
    if (!game::first_frame_) {
        game::first_frame_ = true;
    }
    if (game::first_frame_ && !game::second_frame_) {
        ToggleFullscreen();
        game::second_frame_ = true;
    }
#endif
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
    if (game::paused_) pause_menu::tick_();
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

void game::pause() {
    game::paused_ = true;
    pause_menu::reset_state();
}

void game::unpause() {
    game::paused_ = false;
}
