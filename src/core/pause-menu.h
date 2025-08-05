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
    enum option { resume, restart, volume };
#else
    enum option { resume, restart, volume, fullscreen, quit };
#endif

    static option selected_;
    static bool one_frame_buffer_;

    static void tick_();
    static void render_();
    static void render_text_(string text, float y, bool selected);
    static void render_volume_(float x, float y, bool selected);
    static void select_option_();
    static void change_volume_();
#ifndef WEB
    static void render_fullscreen_(float x, float y, bool selected);
    static void change_fullscreen_();
#endif
};
