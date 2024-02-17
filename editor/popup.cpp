#include "popup.h"
#include "button.h"
#include "defs.h"
#include <raylib.h>
#include <string>

using namespace std;

popup* popup::opened_popup_ = nullptr;
popup* popup::next_popup_ = nullptr;

void popup::empty_confirm_() {
}

void popup::open() {
    popup::next_popup_ = this;
}

void popup::close_current_popup() {
    popup::next_popup_ = nullptr;
}

bool popup::is_popup_opened() {
    return popup::opened_popup_ != nullptr;
}

void popup::tick() {
    if (popup::opened_popup_) {
        popup::opened_popup_->update_btns_();
        popup::opened_popup_->render_();
        if (popup::opened_popup_->confirm_button) {
            popup::opened_popup_->confirm_btn_.tick();
            popup::opened_popup_->close_btn_.tick();
        } else {
            popup::opened_popup_->close_btn_.tick();
        }
    }
    popup::opened_popup_ = popup::next_popup_;
}

void popup::update_btns_() {
    this->confirm_btn_.on_click = this->on_confirm;
    this->close_btn_.on_click = popup::close_current_popup;
    if (this->confirm_button) {
        this->confirm_btn_.label = "yes";
        this->confirm_btn_.rect = (Rectangle){
            EDITOR_WINDOW_SIZE_X / 2 - POPUP_BUTTON_WIDTH / 2 - POPUP_WIDTH / 4,
            EDITOR_WINDOW_SIZE_Y / 2 + POPUP_HEIGHT / 2 - POPUP_PADDING -
                POPUP_BUTTON_HEIGHT,
            POPUP_BUTTON_WIDTH,
            POPUP_BUTTON_HEIGHT};
        this->close_btn_.label = "no";
        this->close_btn_.rect = (Rectangle){
            EDITOR_WINDOW_SIZE_X / 2 - POPUP_BUTTON_WIDTH / 2 + POPUP_WIDTH / 4,
            EDITOR_WINDOW_SIZE_Y / 2 + POPUP_HEIGHT / 2 - POPUP_PADDING -
                POPUP_BUTTON_HEIGHT,
            POPUP_BUTTON_WIDTH,
            POPUP_BUTTON_HEIGHT};
    } else {
        this->close_btn_.label = "ok";
        this->close_btn_.rect =
            (Rectangle){EDITOR_WINDOW_SIZE_X / 2 - POPUP_BUTTON_WIDTH / 2,
                        EDITOR_WINDOW_SIZE_Y / 2 + POPUP_HEIGHT / 2 -
                            POPUP_PADDING - POPUP_BUTTON_HEIGHT,
                        POPUP_BUTTON_WIDTH,
                        POPUP_BUTTON_HEIGHT};
    }
}

void popup::render_() {

    // Darken background
    DrawRectangleRec(
        (Rectangle){0, 0, EDITOR_WINDOW_SIZE_X, EDITOR_WINDOW_SIZE_Y},
        POPUP_BACKGROUND_SCREEN);

    // Draw popup container
    DrawRectangleRec((Rectangle){EDITOR_WINDOW_SIZE_X / 2 - POPUP_WIDTH / 2,
                                 EDITOR_WINDOW_SIZE_Y / 2 - POPUP_HEIGHT / 2,
                                 POPUP_WIDTH,
                                 POPUP_HEIGHT},
                     BG_COLOR);
    DrawRectangleLinesEx(
        (Rectangle){EDITOR_WINDOW_SIZE_X / 2 - POPUP_WIDTH / 2,
                    EDITOR_WINDOW_SIZE_Y / 2 - POPUP_HEIGHT / 2,
                    POPUP_WIDTH,
                    POPUP_HEIGHT},
        1,
        PRIMARY_COLOR);

    // Draw text
    Vector2 line_1_size = MeasureTextEx(
        GetFontDefault(), this->line_1.data(), FONT_SIZE, TEXT_SPACING);
    Vector2 line_2_size = MeasureTextEx(
        GetFontDefault(), this->line_2.data(), FONT_SIZE, TEXT_SPACING);
    DrawText(this->line_1.data(),
             EDITOR_WINDOW_SIZE_X / 2 - (int)line_1_size.x / 2,
             EDITOR_WINDOW_SIZE_Y / 2 + POPUP_PADDING - POPUP_HEIGHT / 2,
             FONT_SIZE,
             FG_COLOR);
    DrawText(this->line_2.data(),
             EDITOR_WINDOW_SIZE_X / 2 - (int)line_2_size.x / 2,
             EDITOR_WINDOW_SIZE_Y / 2 + POPUP_PADDING - POPUP_HEIGHT / 2 +
                 (int)line_1_size.y + POPUP_LINE_SPACING,
             FONT_SIZE,
             FG_COLOR);
}
