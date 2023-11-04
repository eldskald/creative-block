#include "game.h"
#include "game-element.h"
#include <list>

game_element* game::root_ = nullptr;

void tick_game_element(game_element* element) {
    list<game_element*> children = element->get_children();
    list<game_element*>::iterator i;
    for (i = children.begin(); i != children.end(); ++i) {
        tick_game_element(*i);
    }
    element->tick();
}

void game::do_tick_loop() {
    if (game::root_) tick_game_element(game::root_);
}

void game::set_root(game_element* new_root) {
    game::root_ = new_root;
}

game_element* game::get_root() {
    return game::root_;
}
