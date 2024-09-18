#include "core/scene-manager.h"
#include "core/base-unit.h"
#include "core/data-loader.h"
#include "core/game.h"
#include "core/player.h"
#include "core/shadow.h"
#include "engine/game-element.h"
#include <list>
#include <raylib.h>

using namespace std;

scene_manager::scene scene_manager::current_scene_ = scene_manager::START_MENU;
Vector2 scene_manager::player_spawn_point_ = (Vector2){0};
list<input_history> scene_manager::shadow_histories_ = {};
list<shadow*> scene_manager::shadows_ = {};
int scene_manager::level_shadows_limit_ = 1;

void scene_manager::initialize() {
    auto* root = new game_element();
    game::set_root(root);
    list<game_element*> elements = data_loader::load(START_MENU_FILE);
    for (auto element : elements) {
        root->add_child(element);
        auto* player_element = dynamic_cast<player*>(element);
        if (player_element) {
            scene_manager::player_spawn_point_ = player_element->pos;
        }
    }
}

scene_manager::scene scene_manager::get_current_scene() {
    return scene_manager::current_scene_;
}

void scene_manager::respawn_player() {
    auto* new_player = new player();
    new_player->pos = scene_manager::player_spawn_point_;
    game::get_root()->add_child(new_player);
    new_player->emit_respawn_particles();
}

void scene_manager::shadow_pressed(input_history history, player* player) {
    scene_manager::new_shadow_history_(history);
    scene_manager::reset_player_pos_(player);
    scene_manager::spawn_shadows_();
}

void scene_manager::reset_player_pos_(player* player) {
    player->pos = scene_manager::player_spawn_point_;
}

void scene_manager::new_shadow_history_(input_history history) {
    input_history new_history = history;
    scene_manager::shadow_histories_.push_front(new_history);
    if ((int)scene_manager::shadow_histories_.size() >
        scene_manager::level_shadows_limit_) {
        scene_manager::shadow_histories_.pop_back();
    }
}

void scene_manager::change_scene(scene scene) {
    list<game_element*> elements;
    switch (scene) {
    case START_MENU: {
        elements = data_loader::load(START_MENU_FILE);
        scene_manager::current_scene_ = scene_manager::START_MENU;
    }
    case LEVEL_01: {
        elements = data_loader::load(LEVEL_O1_FILE);
        scene_manager::current_scene_ = scene_manager::LEVEL_01;
    }
    }
    game::get_root()->mark_for_deletion();
    auto* new_root = new game_element();
    game::set_root(new_root);
    for (game_element* element : elements) {
        new_root->add_child(element);
        auto* player_element = dynamic_cast<player*>(element);
        if (player_element) {
            scene_manager::player_spawn_point_ = player_element->pos;
        }
    }
}

void scene_manager::spawn_shadows_() {
    while (!scene_manager::shadows_.empty()) {
        scene_manager::shadows_.front()->mark_for_deletion();
        scene_manager::shadows_.pop_front();
    }
    for (auto history : scene_manager::shadow_histories_) {
        auto* new_shadow = new shadow(history);
        new_shadow->pos = scene_manager::player_spawn_point_;
        game::get_root()->add_child(new_shadow);
        new_shadow->emit_spawn_particles();
        scene_manager::shadows_.push_back(new_shadow);
    }
}
