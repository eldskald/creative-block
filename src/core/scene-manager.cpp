#include "core/scene-manager.h"
#include "core/data-loader.h"
#include "engine/game-element.h"
#include "core/game.h"
#include <list>
#include <raylib.h>

using namespace std;

scene_manager::scene scene_manager::current_scene_ = scene_manager::START_MENU;

void scene_manager::initialize() {
    auto* root = new game_element();
    game::set_root(root);
    list<game_element*> elements = data_loader::load(START_MENU_FILE);
    for (auto element : elements) {
        root->add_child(element);
    }
}

scene_manager::scene scene_manager::get_current_scene() {
    return scene_manager::current_scene_;
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
    }
}
