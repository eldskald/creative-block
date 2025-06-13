#include "engine/game-element.h"
#include <list>
#include <raylib.h>
#include <raymath.h>

using namespace std;

list<game_element*> game_element::marked_for_deletion_ = {};
list<game_element*> game_element::to_be_reparented_ = {};

game_element* game_element::get_parent() {
    return this->parent_;
}

list<game_element*> game_element::get_children() {
    list<game_element*> duplicate;
    for (auto child : this->children_) {
        duplicate.push_back(child);
    }
    return duplicate;
}

void game_element::add_child(game_element* element) {
    if (element->parent_) return;
    element->parent_ = this;
    this->children_.push_back(element);
    if (this->is_on_tree_) game_element::trigger_enter_(element);
}

void game_element::remove_child(game_element* child) {
    if (child->parent_ != this) return;
    if (child->is_on_tree_) game_element::trigger_exit_(child);
    this->children_.remove(child);
    child->parent_ = nullptr;
}

void game_element::reparent(game_element* new_parent) {
    if (!this->is_on_tree_) return;
    this->next_parent_ = new_parent;
    game_element::to_be_reparented_.push_back(this);
}

bool game_element::descends_from(game_element* element) {
    game_element* curr_parent = this->parent_;
    while (curr_parent != nullptr) {
        if (curr_parent == element) return true;
        curr_parent = curr_parent->parent_;
    }
    return false;
}

bool game_element::is_on_tree() {
    return this->is_on_tree_;
}

void game_element::mark_for_deletion() {
    game_element::rec_mark_for_deletion_(this);
}

Vector2 game_element::get_global_pos() {
    Vector2 global_pos = this->pos;
    game_element* parent = this->get_parent();
    while (parent) {
        global_pos = Vector2Add(global_pos, parent->pos);
        parent = parent->get_parent();
    }
    return global_pos;
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

void game_element::on_player_death_() {
    return;
}

void game_element::on_player_shadow_() {
    return;
}

void game_element::trigger_enter_(game_element* element) {
    element->is_on_tree_ = true;
    element->enter_();
    for (auto child : element->children_) {
        game_element::trigger_enter_(child);
    }
}

void game_element::trigger_exit_(game_element* element) {
    for (auto child : element->children_) {
        game_element::trigger_exit_(child);
    }
    element->exit_();
    element->is_on_tree_ = false;
}

void game_element::trigger_tick_(game_element* element) {
    for (auto child : element->children_) {
        game_element::trigger_tick_(child);
    }
    element->tick_();
}

void game_element::trigger_on_player_death_(game_element* element) {
    for (auto child : element->children_) {
        game_element::trigger_on_player_death_(child);
    }
    element->on_player_death_();
}

void game_element::trigger_on_player_shadow_(game_element* element) {
    for (auto child : element->children_) {
        game_element::trigger_on_player_shadow_(child);
    }
    element->on_player_shadow_();
}

void game_element::rec_mark_for_deletion_(game_element* element) {
    if (element->is_marked_for_deletion_) return;
    game_element::marked_for_deletion_.push_back(element);
    element->is_marked_for_deletion_ = true;
    for (auto child : element->children_) {
        game_element::rec_mark_for_deletion_(child);
    }
}

void game_element::delete_marked_() {
    if (game_element::marked_for_deletion_.empty()) return;
    for (auto element : game_element::marked_for_deletion_) {
        if (element->is_on_tree_) element->exit_();
        if (element->parent_) {
            if (!element->parent_->is_marked_for_deletion_) {
                element->parent_->children_.remove(element);
            }
        }
        delete element;
    }
    game_element::marked_for_deletion_.clear();
}

void game_element::reparent_elements_() {
    if (game_element::to_be_reparented_.empty()) return;
    while (!game_element::to_be_reparented_.empty()) {
        auto* element = game_element::to_be_reparented_.front();
        game_element::to_be_reparented_.pop_front();
        element->pos = Vector2Subtract(element->get_global_pos(),
                                       element->next_parent_->get_global_pos());
        element->parent_->remove_child(element);
        element->next_parent_->add_child(element);
    }
}
