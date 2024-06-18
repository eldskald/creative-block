#include "button.h"
#include "defs.h"
#include "editor.h"
#include <raylib.h>
#include <string>

void button::empty_on_click_() {
}

bool button::is_being_hovered_() {
    bool x_check = (GetMouseX() >= (int)this->rect.x) &&
                   (GetMouseX() <= (int)this->rect.x + (int)this->rect.width);
    bool y_check = (GetMouseY() >= (int)this->rect.y) &&
                   (GetMouseY() <= (int)this->rect.y + (int)this->rect.height);
    return x_check && y_check;
}

void button::detect_clicks_() {
    if (editor::mouse_disabled) {
        this->being_clicked_ = false;
        return;
    }
    if (this->is_being_hovered_() && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        this->being_clicked_ = true;
    } else if (this->being_clicked_ && IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        this->being_clicked_ = false;
        if (this->toggle_mode) this->pressed_ = !this->pressed_;
        this->on_click();
    } else {
        this->being_clicked_ = false;
    }
}

bool button::is_pressed() {
    return this->toggle_mode && this->pressed_;
}

void button::set_toggle(bool value) {
    if (this->toggle_mode) this->pressed_ = value;
}

void button::render_() {
    Color col = PRIMARY_COLOR;
    if (this->pressed_) col = FOCUSED_COLOR;
    if (this->is_being_hovered_() && !editor::mouse_disabled) {
        col = this->being_clicked_ ? FOCUSED_COLOR : HOVERED_COLOR;
    }
    if (this->being_clicked_ || this->pressed_) {
        DrawRectangleLinesEx((Rectangle){this->rect.x + 2,
                                         this->rect.y + 2,
                                         this->rect.width - 4,
                                         this->rect.height - 4},
                             1,
                             col);
        Vector2 text_size = MeasureTextEx(
            GetFontDefault(), this->label.c_str(), FONT_SIZE - 2, TEXT_SPACING);
        DrawText(this->label.c_str(),
                 (int)this->rect.x + (int)this->rect.width / 2 -
                     (int)text_size.x / 2,
                 (int)this->rect.y + (int)this->rect.height / 2 -
                     (int)text_size.y / 2,
                 FONT_SIZE - 2,
                 col);
    } else {
        DrawRectangleLinesEx(this->rect, 1, col);
        Vector2 text_size = MeasureTextEx(
            GetFontDefault(), this->label.c_str(), FONT_SIZE, TEXT_SPACING);
        DrawText(this->label.c_str(),
                 (int)this->rect.x + (int)this->rect.width / 2 -
                     (int)text_size.x / 2,
                 (int)this->rect.y + (int)this->rect.height / 2 -
                     (int)text_size.y / 2,
                 FONT_SIZE,
                 col);
    }

    // Set mouse cursor when being hovered
    if (this->is_being_hovered_() && !editor::mouse_disabled) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    };
}

void button::tick() {
    this->render_();
    if (editor::mouse_disabled) return;
    this->detect_clicks_();
}
