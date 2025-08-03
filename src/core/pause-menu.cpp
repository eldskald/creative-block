#include "core/pause-menu.h"
#include "core/game.h"
#include "core/scene-manager.h"
#include "defs.h"
#include "engine/inputs.h"
#include "engine/text.h"
#include <cmath>
#include <raylib.h>
#include <string>

using namespace std;

#ifndef WEB
const int OPTIONS_TOTAL = 3;
#else
const int OPTIONS_TOTAL = 2;
#endif

pause_menu::option pause_menu::selected_ = pause_menu::option::resume;

void pause_menu::tick_() {
    if (inputs::is_action_pressed(inputs::action::down))
        pause_menu::selected_ =
            (pause_menu::option)((pause_menu::selected_ + 1) % OPTIONS_TOTAL);
    if (inputs::is_action_pressed(inputs::action::up))
        pause_menu::selected_ =
            (pause_menu::option)((pause_menu::selected_ - 1) % OPTIONS_TOTAL);

    if (inputs::is_action_pressed(inputs::action::accept))
        pause_menu::select_option_();
}

void pause_menu::reset_state() {
    pause_menu::selected_ = pause_menu::option::resume;
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
#ifndef WEB
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
#ifndef WEB
    case pause_menu::option::quit: {
        game::close_game();
        break;
    }
#endif
    }
}
