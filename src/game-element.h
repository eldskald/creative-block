#pragma once
#include <list>

using namespace std;

class GameElement {
public:
    GameElement();
    ~GameElement();

    GameElement* get_parent();
    list<GameElement*> get_children();
    void add_child(GameElement* element);
    void remove_child(GameElement* child);

    virtual void tick();

private:
    GameElement* _parent;
    list<GameElement*> _children;
};
