#pragma once
#include "core/base-unit.h"
#include "core/player.h"
#include "core/shadow.h"
#include "imports.h"
#include <list>
#include <raylib.h>
#include <unordered_map>

class game;

class scene_manager {
    friend class game;

public:
    enum scene SCENES_ENUM;

    static void initialize();
    static scene get_current_scene();
    static void set_shadows_limit(int total);
    static int get_shadows_limit();
    static void respawn_player();
    static void next_scene();
    static void shadow_pressed(input_history history, player* player);

private:
    // static unordered_map<scene, scene> next_scenes_;
    static unordered_map<scene, const char*> scenes_map_;
    static scene current_scene_;
    static Vector2 player_spawn_point_;
    static list<input_history> shadow_histories_;
    static int level_shadows_limit_;
    static list<shadow*> shadows_;
    static bool next_scene_called_;

    static void reset_player_pos_(player* player);
    static void new_shadow_history_(input_history history);
    static void spawn_shadows_();
    static void load_new_scene_();
};
