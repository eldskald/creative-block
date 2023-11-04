#include "game-element.h"
#include <iostream>
#include <list>

using namespace std;

game_element::game_element() {
    this->parent_ = nullptr;
}

game_element::~game_element() {
    if (this->parent_) this->parent_->remove_child(this);
    list<game_element*>::iterator i;
    for (i = this->children_.begin(); i != this->children_.end(); ++i) {
        delete *i;
    }
}

game_element* game_element::get_parent() {
    return this->parent_;
}

list<game_element*> game_element::get_children() {
    list<game_element*> duplicate;
    list<game_element*>::iterator i;
    for (i = this->children_.begin(); i != this->children_.end(); ++i) {
        duplicate.push_back(*i);
    }
    return duplicate;
}

void game_element::add_child(game_element* element) {
    if (element->parent_) element->parent_->children_.remove(element);
    element->parent_ = this;
    this->children_.push_back(element);
}

void game_element::remove_child(game_element* child) {
    if (child->parent_ == this) {
        this->children_.remove(child);
        child->parent_ = nullptr;
    }
}

void game_element::tick() {
}
