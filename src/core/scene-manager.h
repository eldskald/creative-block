#pragma once
#include <raylib.h>

class scene_manager {
public:
    enum scene { START_MENU, LEVEL_01 };

    static void initialize();
    static scene get_current_scene();
    static void spawn_player();
    static void change_scene(scene scene);

private:
    static scene current_scene_;
    static Vector2 player_spawn_point_;
};
