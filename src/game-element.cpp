#include "game-element.h"
#include <list>
#include <raymath.h>

using namespace std;

game_element::game_element() {
    this->parent_ = nullptr;
    this->pos = Vector2Zero();
    this->global_pos_ = Vector2Zero();
    this->is_on_tree_ = false;
}

game_element::~game_element() {
    if (this->parent_) this->parent_->remove_child(this);
    for (auto i = this->children_.begin(); i != this->children_.end(); ++i) {
        delete *i;
    }
}

game_element* game_element::get_parent() {
    return this->parent_;
}

list<game_element*> game_element::get_children() {
    list<game_element*> duplicate;
    for (auto i = this->children_.begin(); i != this->children_.end(); ++i) {
        duplicate.push_back(*i);
    }
    return duplicate;
}

void game_element::trigger_enter_(game_element* element) {
    for (auto i = element->children_.begin(); i != element->children_.end();
         ++i) {
        game_element::trigger_enter_(*i);
    }
    element->is_on_tree_ = true;
    element->enter_();
}

void game_element::add_child(game_element* element) {
    if (element->parent_) {
        return;
    }
    element->parent_ = this;
    this->children_.push_back(element);
    element->pos = Vector2Subtract(element->pos, this->global_pos_);
    if (this->is_on_tree_) game_element::trigger_enter_(element);
}

void game_element::trigger_exit_(game_element* element) {
    for (auto i = element->children_.begin(); i != element->children_.end();
         ++i) {
        game_element::trigger_exit_(*i);
    }
    element->is_on_tree_ = false;
    element->exit_();
}

void game_element::remove_child(game_element* child) {
    if (child->parent_ == this) {
        this->children_.remove(child);
        child->parent_ = nullptr;
        child->pos = child->global_pos_;
        game_element::trigger_exit_(child);
    }
}

void game_element::update_pos_() {
    if (this->parent_) {
        this->global_pos_ = Vector2Add(this->parent_->global_pos_, this->pos);
    } else {
        this->global_pos_ = this->pos;
    }
}

void game_element::enter_() {
    return;
}

void game_element::exit_() {
    return;
}

void game_element::tick_() {
    return;
}
