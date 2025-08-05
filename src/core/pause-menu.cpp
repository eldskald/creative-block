#include "core/pause-menu.h"
#include "core/game.h"
#include "core/scene-manager.h"
#include "defs.h"
#include "engine/inputs.h"
#include "engine/sfx.h"
#include "engine/text.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <string>

using namespace std;

#ifndef WEB
const int OPTIONS_TOTAL = 5;
#else
const int OPTIONS_TOTAL = 3;
#endif

pause_menu::option pause_menu::selected_ = pause_menu::option::resume;
bool pause_menu::one_frame_buffer_ = true;

void pause_menu::tick_() {
    if (pause_menu::one_frame_buffer_) {
        sfx::play(sfx::select);
        pause_menu::one_frame_buffer_ = false;
        return;
    }

    if (inputs::is_action_pressed(inputs::action::down)) {
        pause_menu::selected_ =
            (pause_menu::option)((pause_menu::selected_ + 1) % OPTIONS_TOTAL);
        sfx::play(sfx::menu_navigate);
    }
    if (inputs::is_action_pressed(inputs::action::up)) {
        pause_menu::selected_ =
            (pause_menu::option)((pause_menu::selected_ + OPTIONS_TOTAL - 1) %
                                 OPTIONS_TOTAL);
        sfx::play(sfx::menu_navigate);
    }
    if (inputs::is_action_pressed(inputs::action::accept)) {
        pause_menu::select_option_();
        sfx::play(sfx::select);
    }

    if (pause_menu::selected_ == pause_menu::option::volume)
        pause_menu::change_volume_();

#ifndef WEB
    if (pause_menu::selected_ == pause_menu::option::fullscreen)
        pause_menu::change_fullscreen_();
#endif
}

void pause_menu::reset_state() {
    pause_menu::selected_ = pause_menu::option::resume;
    pause_menu::one_frame_buffer_ = true;
}

void pause_menu::render_() {
    const int x = (WINDOW_SIZE_X - PAUSE_MENU_WIDTH) / 2;
    const int y = (WINDOW_SIZE_Y - PAUSE_MENU_HEIGHT) / 2;
    DrawRectangle(x, y, PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT, BLACK);
    DrawRectangleLinesEx((Rectangle){x, y, PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT},
                         PAUSE_MENU_LINE_WIDTH,
                         MASK_MAIN_COLOR);

    pause_menu::render_text_(PAUSE_MENU_TITLE, PAUSE_MENU_TITLE_Y + y, false);
    pause_menu::render_text_(PAUSE_MENU_RESUME,
                             PAUSE_MENU_RESUME_Y + y,
                             pause_menu::selected_ ==
                                 pause_menu::option::resume);
    pause_menu::render_text_(PAUSE_MENU_RESTART,
                             PAUSE_MENU_RESTART_Y + y,
                             pause_menu::selected_ ==
                                 pause_menu::option::restart);
    pause_menu::render_volume_(x,
                               PAUSE_MENU_VOLUME_Y + y,
                               pause_menu::selected_ ==
                                   pause_menu::option::volume);
#ifndef WEB
    pause_menu::render_fullscreen_(x,
                                   PAUSE_MENU_FULLSCREEN_Y + y,
                                   pause_menu::selected_ ==
                                       pause_menu::option::fullscreen);
    pause_menu::render_text_(PAUSE_MENU_QUIT,
                             PAUSE_MENU_QUIT_Y + y,
                             pause_menu::selected_ == pause_menu::option::quit);
#endif
}

void pause_menu::render_text_(string text, float y, bool selected) {
    Vector2 size = MeasureTextEx(
        text::label_font_, text.data(), LABEL_FONT_SIZE, LABEL_FONT_SPACING);
    Vector2 pos =
        (Vector2){floor((WINDOW_SIZE_X - size.x) / 2), floor(y - size.y / 2)};

    DrawTextEx(text::label_font_,
               text.data(),
               pos,
               LABEL_FONT_SIZE,
               LABEL_FONT_SPACING,
               selected ? MASK_SHADOW_COLOR : MASK_MAIN_COLOR);
}

void pause_menu::render_volume_(float x, float y, bool selected) {
    const float convert_to_percent = 100.0f;
    string volume =
        to_string((int)(GetMasterVolume() * convert_to_percent)) + "%";
    Vector2 size = MeasureTextEx(
        text::label_font_, volume.data(), LABEL_FONT_SIZE, LABEL_FONT_SPACING);

    DrawTextEx(text::label_font_,
               PAUSE_MENU_VOLUME,
               (Vector2){x + PAUSE_MENU_PADDING, floor(y - size.y / 2)},
               LABEL_FONT_SIZE,
               LABEL_FONT_SPACING,
               selected ? MASK_SHADOW_COLOR : MASK_MAIN_COLOR);

    DrawTextEx(text::label_font_,
               volume.data(),
               (Vector2){x + PAUSE_MENU_WIDTH - PAUSE_MENU_PADDING - size.x,
                         floor(y - size.y / 2)},
               LABEL_FONT_SIZE,
               LABEL_FONT_SPACING,
               selected ? MASK_SHADOW_COLOR : MASK_MAIN_COLOR);
}

void pause_menu::select_option_() {
    switch (pause_menu::selected_) {
    case pause_menu::option::resume: {
        game::unpause();
        break;
    }
    case pause_menu::option::restart: {
        scene_manager::reload_scene();
        game::unpause();
        break;
    }
    case pause_menu::option::volume: {
        if (GetMasterVolume() > 0.0f)
            SetMasterVolume(0.0f);
        else
            SetMasterVolume(1.0f);
        break;
    }
#ifndef WEB
    case pause_menu::fullscreen: {
        ToggleFullscreen();
        break;
    }
    case pause_menu::option::quit: {
        game::close_game();
        break;
    }
#endif
    }
}

void pause_menu::change_volume_() {
    const float scale = 10.0f;
    int volume = (int)(GetMasterVolume() * scale);
    if (inputs::is_action_pressed(inputs::action::left)) {
        int new_volume = volume - 1;
        SetMasterVolume(clamp((float)new_volume / scale, 0.0f, 1.0f));
        sfx::play(sfx::select);
    }
    if (inputs::is_action_pressed(inputs::action::right)) {
        int new_volume = volume + 1;
        SetMasterVolume(clamp((float)new_volume / scale, 0.0f, 1.0f));
        sfx::play(sfx::select);
    }
}

#ifndef WEB
void pause_menu::change_fullscreen_() {
    if (inputs::is_action_pressed(inputs::action::left) ||
        inputs::is_action_pressed(inputs::action::right)) {
        ToggleFullscreen();
        sfx::play(sfx::select);
    }
}

void pause_menu::render_fullscreen_(float x, float y, bool selected) {
    string fullscreen = IsWindowFullscreen() ? "On" : "Off";
    Vector2 size = MeasureTextEx(text::label_font_,
                                 fullscreen.data(),
                                 LABEL_FONT_SIZE,
                                 LABEL_FONT_SPACING);

    DrawTextEx(text::label_font_,
               PAUSE_MENU_FULLSCREEN,
               (Vector2){x + PAUSE_MENU_PADDING, floor(y - size.y / 2)},
               LABEL_FONT_SIZE,
               LABEL_FONT_SPACING,
               selected ? MASK_SHADOW_COLOR : MASK_MAIN_COLOR);

    DrawTextEx(text::label_font_,
               fullscreen.data(),
               (Vector2){x + PAUSE_MENU_WIDTH - PAUSE_MENU_PADDING - size.x,
                         floor(y - size.y / 2)},
               LABEL_FONT_SIZE,
               LABEL_FONT_SPACING,
               selected ? MASK_SHADOW_COLOR : MASK_MAIN_COLOR);
}
#endif
