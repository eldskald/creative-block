#pragma once
#include <string>

using namespace std;

class renderer;
class game;

class pause_menu {
    friend class renderer;
    friend class game;

public:
    static void reset_state();

private:
#ifdef WEB
    enum option { resume, restart };
#else
    enum option { resume, restart, quit };
#endif

    static option selected_;

    static void tick_();
    static void render_();
    static void render_text_(string text, float y, bool selected);
    static void select_option_();
};
