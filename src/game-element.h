#pragma once
#include <list>
#include <raylib.h>

using namespace std;

class game;

class game_element {
    friend class game;

public:
    Vector2 pos{(Vector2){0}};

    game_element() = default;
    game_element(const game_element&) = default;
    game_element(game_element&&) = delete;
    game_element& operator=(const game_element&) = default;
    game_element& operator=(game_element&&) = delete;
    virtual ~game_element() = default;

    game_element* get_parent();
    list<game_element*> get_children();
    void add_child(game_element* element);
    void remove_child(game_element* child);
    void mark_for_deletion();
    bool descends_from(game_element* element);
    bool is_on_tree();
    Vector2 get_global_pos();

protected:
    virtual void enter_();
    virtual void exit_();
    virtual void tick_();

private:
    game_element* parent_{nullptr};
    list<game_element*> children_;
    bool is_on_tree_{false};
    bool is_marked_for_deletion_{false};

    static list<game_element*> marked_for_deletion_;

    static void trigger_enter_(game_element* element);
    static void trigger_exit_(game_element* element);
    static void trigger_tick_(game_element* element);
    static void rec_mark_for_deletion_(game_element* element);
    static void delete_marked_();
};
