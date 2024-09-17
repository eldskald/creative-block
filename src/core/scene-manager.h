#pragma once
#include "core/player.h"
#include "core/shadow.h"
#include "core/base-unit.h"
#include <raylib.h>
#include <list>

class scene_manager {
public:
    enum scene { START_MENU, LEVEL_01 };

    static void initialize();
    static scene get_current_scene();
    static void respawn_player();
    static void remove_shadow(shadow* shadow);
    static void change_scene(scene scene);
    static void shadow_pressed(input_history history, player* player);

private:
    static scene current_scene_;
    static Vector2 player_spawn_point_;
    static list<input_history> shadow_histories_;
    static int level_shadows_limit_;
    static list<shadow*> shadows_;

    static void reset_player_pos_(player* player);
    static void new_shadow_history_(input_history history);
    static void spawn_shadows_();
};
