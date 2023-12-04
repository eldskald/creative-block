#include "game.h"
#include "game-element.h"
#include "sprite.h"
#include <list>
#include <raylib.h>

game_element* game::root_ = nullptr;

void game::tick_game_element_(game_element* element) {
    list<game_element*>::iterator i;
    for (i = element->children_.begin(); i != element->children_.end(); ++i) {
        tick_game_element_(*i);
    }
    element->tick_();
}

void game::update_game_element_pos_(game_element* element) {
    element->update_pos_();
    list<game_element*>::iterator i;
    for (i = element->children_.begin(); i != element->children_.end(); ++i) {
        update_game_element_pos_(*i);
    }
}

void game::initial_setup() {
    sprite::atlas_ = LoadTexture(SPRITESHEET_FILE);
}

void game::do_game_loop() {
    if (!game::root_) return;
    game::update_game_element_pos_(game::root_);
    game::tick_game_element_(game::root_);
}

void game::set_root(game_element* new_root) {
    game::root_ = new_root;
}

game_element* game::get_root() {
    return game::root_;
}
