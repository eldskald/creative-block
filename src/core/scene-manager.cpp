#include "core/scene-manager.h"
#include "core/base-unit.h"
#include "core/data-loader.h"
#include "core/game.h"
#include "core/player.h"
#include "core/shadow.h"
#include "engine/game-element.h"
#include "engine/renderer.h"
#include "imports.h"
#include <list>
#include <raylib.h>
#include <unordered_map>

using namespace std;

unordered_map<scene_manager::scene, const char*> scene_manager::scenes_map_ =
    SCENES_PATHS;
scene_manager::scene scene_manager::current_scene_ = scene_manager::opening;
Vector2 scene_manager::player_spawn_point_ = (Vector2){0};
list<input_history> scene_manager::shadow_histories_ = {};
list<shadow*> scene_manager::shadows_ = {};
int scene_manager::level_shadows_limit_ = 1;
bool scene_manager::next_scene_called_ = false;

void scene_manager::initialize() {
    auto* root = new game_element();
#ifdef SCENE
    list<game_element*> elements =
        data_loader::load(scene_manager::scenes_map_[(scene)SCENE]);
    scene_manager::current_scene_ = (scene)SCENE;
#else
    list<game_element*> elements =
        data_loader::load(scene_manager::scenes_map_[opening]);
#endif
    for (auto element : elements) {
        root->add_child(element);
        auto player_element = dynamic_cast<player*>(element);
        if (player_element) {
            scene_manager::player_spawn_point_ = player_element->pos;
        }
    }
    game::set_root(root);
}

scene_manager::scene scene_manager::get_current_scene() {
    return scene_manager::current_scene_;
}

void scene_manager::set_shadows_limit(int total) {
    scene_manager::level_shadows_limit_ = total;
}

int scene_manager::get_shadows_limit() {
    return scene_manager::level_shadows_limit_;
}

void scene_manager::respawn_player() {
    auto* new_player = new player();
    new_player->pos = scene_manager::player_spawn_point_;
    game::get_root()->add_child(new_player);
    game_element::trigger_on_player_respawn_(game::get_root());
}

void scene_manager::shadow_pressed(input_history history, player* player) {
    if (scene_manager::level_shadows_limit_ == 0) return;
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

void scene_manager::next_scene() {
    scene_manager::next_scene_called_ = true;
    renderer::reset_water_waves();
    for (game_element* element : game::get_root()->get_children()) {
        element->mark_for_deletion();
    }
}

void scene_manager::load_new_scene_() {
    if (!scene_manager::next_scene_called_) return;
    auto next_level = (scene)(((int)scene_manager::current_scene_ + 1) %
                              scene_manager::scenes_map_.size());
    list<game_element*> elements =
        data_loader::load(scene_manager::scenes_map_[next_level]);
    for (game_element* element : elements) {
        game::get_root()->add_child(element);
        auto* player_element = dynamic_cast<player*>(element);
        if (player_element) {
            scene_manager::player_spawn_point_ = player_element->pos;
        }
    }
    scene_manager::shadows_.clear();
    scene_manager::current_scene_ = next_level;
    scene_manager::next_scene_called_ = false;
}

void scene_manager::spawn_shadows_() {
    game_element::trigger_on_player_shadow_(game::get_root());
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
