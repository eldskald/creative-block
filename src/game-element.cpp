#include "game-element.h"
#include <iostream>
#include <list>

using namespace std;

GameElement::GameElement() {
    this->_parent = nullptr;
}

GameElement::~GameElement() {
    if (this->_parent) this->_parent->remove_child(this);
    list<GameElement*>::iterator i;
    for (i = this->_children.begin(); i != this->_children.end(); ++i) {
        delete *i;
    }
}

GameElement* GameElement::get_parent() {
    return this->_parent;
}

list<GameElement*> GameElement::get_children() {
    list<GameElement*> duplicate;
    list<GameElement*>::iterator i;
    for (i = this->_children.begin(); i != this->_children.end(); ++i) {
        duplicate.push_back(*i);
    }
    return duplicate;
}

void GameElement::add_child(GameElement* element) {
    if (element->_parent) element->_parent->_children.remove(element);
    element->_parent = this;
    this->_children.push_back(element);
}

void GameElement::remove_child(GameElement* child) {
    if (child->_parent == this) {
        this->_children.remove(child);
        child->_parent = nullptr;
    }
}

void GameElement::tick() {
}
