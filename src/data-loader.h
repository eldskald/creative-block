#pragma once
#include "game-element.h"
#include <list>

class data_loader {
public:
    static list<game_element*> load(const char* file);
};
