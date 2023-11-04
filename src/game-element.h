#pragma once
#include <list>

using namespace std;

class game_element {
public:
    game_element();
    ~game_element();

    game_element* get_parent();
    list<game_element*> get_children();
    void add_child(game_element* element);
    void remove_child(game_element* child);

    virtual void tick();

private:
    game_element* parent_;
    list<game_element*> children_;
};
