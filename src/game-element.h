#pragma once
#include <list>
#include <raylib.h>

using namespace std;

class game;

class game_element {
    friend class game;

public:
    Vector2 pos;

    game_element();
    ~game_element();

    game_element* get_parent();
    list<game_element*> get_children();
    void add_child(game_element* element);
    void remove_child(game_element* child);
    bool descends_from(game_element* element);
    bool is_on_tree();
    Vector2 get_global_pos();

protected:
    virtual void enter_();
    virtual void exit_();
    virtual void tick_();

private:
    game_element* parent_;
    list<game_element*> children_;
    bool is_on_tree_;

    static void trigger_enter_(game_element* element);
    static void trigger_exit_(game_element* element);
    static void trigger_tick_(game_element* element);
};
