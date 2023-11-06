#include "game-element.h"
#include <list>
#include <raymath.h>

using namespace std;

game_element::game_element() {
    this->parent_ = nullptr;
    this->pos = Vector2Zero();
    this->global_pos_ = Vector2Zero();
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
    if (element->parent_) {
        element->parent_->children_.remove(element);
        element->pos = element->global_pos_;
    }
    element->parent_ = this;
    this->children_.push_back(element);
    element->pos = Vector2Subtract(element->pos, this->global_pos_);
}

void game_element::remove_child(game_element* child) {
    if (child->parent_ == this) {
        this->children_.remove(child);
        child->parent_ = nullptr;
        child->pos = child->global_pos_;
    }
}

void game_element::update_pos_() {
    if (this->parent_) {
        this->global_pos_ = Vector2Add(this->parent_->global_pos_, this->pos);
    } else {
        this->global_pos_ = this->pos;
    }
}

void game_element::tick_() {
}
