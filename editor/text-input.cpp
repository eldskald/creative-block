#include "text-input.h"
#include "defs.h"
#include <iostream>
#include <raylib.h>
#include <string>

bool text_input::is_being_hovered_() {
    bool x_check = (GetMouseX() >= (int)this->rect.x) &&
                   (GetMouseX() <= (int)this->rect.x + (int)this->rect.width);
    bool y_check = (GetMouseY() >= (int)this->rect.y) &&
                   (GetMouseY() <= (int)this->rect.y + (int)this->rect.height);
    return x_check && y_check;
}

void text_input::detect_clicks_() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        this->cursor_blink_time_ = 0.0;
        this->focused_ = this->is_being_hovered_();
    }
}

void text_input::update_input_() {
    int input = GetCharPressed();
    if (input) {
        this->input_.push_back(input);
    } else if (IsKeyPressed(KEY_BACKSPACE) && !(this->input_.empty())) {
        this->input_.pop_back();
    }
}

void text_input::render_() {
    Color col = PRIMARY_COLOR;
    Color input_col = FG_COLOR;
    input_col.a = FG_COLOR.a / 2;
    if (this->is_being_hovered_()) {
        col = HOVERED_COLOR;
        input_col.a = FG_COLOR.a * 3 / 4;
    }
    if (this->focused_) {
        col = FOCUSED_COLOR;
        input_col.a = FG_COLOR.a;
    }

    // Render label
    string label_render = this->label + ':';
    Vector2 label_size = MeasureTextEx(
        GetFontDefault(), label_render.c_str(), FONT_SIZE, TEXT_SPACING);
    DrawText(label_render.c_str(),
             (int)this->rect.x + INPUT_MARGIN,
             (int)this->rect.y + (int)this->rect.height / 2 -
                 (int)label_size.y / 2,
             FONT_SIZE,
             col);

    // Render underline
    Vector2 start_pos = (Vector2){this->rect.x + INPUT_MARGIN,
                                  this->rect.y + this->rect.height / 2 +
                                      label_size.y / 2 + INPUT_UNDERLINE_DIST};
    Vector2 end_pos = (Vector2){this->rect.x + this->rect.width - INPUT_MARGIN,
                                this->rect.y + this->rect.height / 2 +
                                    label_size.y / 2 + INPUT_UNDERLINE_DIST};
    DrawLineEx(start_pos, end_pos, 2, col);

    // Render input
    char* input_text = LoadUTF8(this->input_.data(), (int)this->input_.size());
    Vector2 input_size =
        MeasureTextEx(GetFontDefault(), input_text, FONT_SIZE, TEXT_SPACING);
    DrawText(input_text,
             (int)this->rect.x + (int)label_size.x + INPUT_MARGIN * 3,
             (int)this->rect.y + (int)this->rect.height / 2 -
                 (int)input_size.y / 2,
             FONT_SIZE,
             input_col);
    UnloadUTF8(input_text);

    // Render cursor
    if (this->focused_) {
        this->cursor_blink_time_ += GetFrameTime();
        if (this->cursor_blink_time_ >= INPUT_BLINK_TIME) {
            Vector2 cursor_top = (Vector2){
                this->rect.x + label_size.x + input_size.x + INPUT_MARGIN * 3 +
                    TEXT_SPACING,
                this->rect.y + this->rect.height / 2 - label_size.y / 2};
            Vector2 cursor_bottom = (Vector2){
                this->rect.x + label_size.x + input_size.x + INPUT_MARGIN * 3 +
                    TEXT_SPACING,
                this->rect.y + this->rect.height / 2 + label_size.y / 2};
            DrawLineEx(cursor_top, cursor_bottom, 2, input_col);
        }
        if (this->cursor_blink_time_ >= INPUT_BLINK_TIME * 2) {
            this->cursor_blink_time_ -= INPUT_BLINK_TIME * 2;
        }
    }

    // Set mouse cursor when being hovered
    if (this->is_being_hovered_()) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    };
}

void text_input::tick() {
    this->render_();
    this->detect_clicks_();
    if (this->focused_) this->update_input_();
}
