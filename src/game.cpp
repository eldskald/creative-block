#include "game.h"
#include "game-element.h"
#include <list>

GameElement* Game::_root = nullptr;

void tick_game_element(GameElement* element) {
    list<GameElement*> children = element->get_children();
    list<GameElement*>::iterator i;
    for (i = children.begin(); i != children.end(); ++i) {
        tick_game_element(*i);
    }
    element->tick();
}

void Game::do_tick_loop() {
    if (Game::_root) tick_game_element(Game::_root);
}

void Game::set_root(GameElement* new_root) {
    Game::_root = new_root;
}

GameElement* Game::get_root() {
    return Game::_root;
}
