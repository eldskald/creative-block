#include "text-input.h"
#include "defs.h"
#include <raylib.h>
#include <string>

string text_input::get_input() {
    return this->input_text_;
}

bool text_input::is_being_hovered_() {
    bool x_check = (GetMouseX() >= (int)this->rect.x) &&
                   (GetMouseX() <= (int)this->rect.x + (int)this->rect.width);
    bool y_check = (GetMouseY() >= (int)this->rect.y) &&
                   (GetMouseY() <= (int)this->rect.y + (int)this->rect.height);
    return x_check && y_check;
}

void text_input::detect_clicks_() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        this->cursor_blink_time_ = INPUT_BLINK_TIME;
        this->focused_ = this->is_being_hovered_();

        // Position the cursor on the mouse
        string label_render = this->label + ':';
        Vector2 label_size = MeasureTextEx(
            GetFontDefault(), label_render.data(), FONT_SIZE, TEXT_SPACING);
        if (GetMouseX() >=
            (int)this->rect.x + INPUT_MARGIN + (int)label_size.x) {
            this->cursor_pos_ = 0;
            for (int i = (int)this->input_lengths_.size() - 1; i >= 0; i--) {
                if (GetMouseX() >= (int)this->rect.x + INPUT_MARGIN +
                                       (int)label_size.x +
                                       (int)this->input_lengths_.at(i)) {
                    this->cursor_pos_ = i;
                    break;
                }
            }
        }
    }
}

auto text_input::hold_key_map_(int key) -> void (*)(text_input*) {
    switch (key) {
    case KEY_BACKSPACE:
        return text_input::erase_back_;
    case KEY_LEFT:
        return text_input::move_cursor_left_;
    case KEY_RIGHT:
        return text_input::move_cursor_right_;
    case KEY_HOME:
        return text_input::move_cursor_home_;
    case KEY_END:
        return text_input::move_cursor_end_;
    default:
        return nullptr;
    }
}

void text_input::erase_back_(text_input* input) {
    if (input->cursor_pos_ > 0) {
        input->input_codepoints_.erase(input->input_codepoints_.begin() +
                                       input->cursor_pos_ - 1);
        input->cursor_pos_--;
        input->update_inputs_();
        input->cursor_blink_time_ = INPUT_BLINK_TIME;
    }
}

void text_input::move_cursor_left_(text_input* input) {
    if (input->cursor_pos_ > 0) {
        input->cursor_pos_--;
        input->cursor_blink_time_ = INPUT_BLINK_TIME;
    }
}

void text_input::move_cursor_right_(text_input* input) {
    if (input->cursor_pos_ < input->input_lengths_.size() - 1) {
        input->cursor_pos_++;
        input->cursor_blink_time_ = INPUT_BLINK_TIME;
    }
}

void text_input::move_cursor_home_(text_input* input) {
    input->cursor_pos_ = 0;
}

void text_input::move_cursor_end_(text_input* input) {
    input->cursor_pos_ = (int)input->input_text_.size();
}

void text_input::get_user_input_() {

    // Typing logic
    int input = GetCharPressed();
    if (input) {
        if (this->input_codepoints_.size() < INPUT_MAX_LENGTH) {
            this->input_codepoints_.insert(
                this->input_codepoints_.begin() + this->cursor_pos_, input);
            this->cursor_pos_++;
            this->update_inputs_();
        }
    } else {

        // Other buttons logic (backspace, delete and arrows)
        vector<int> keys = {
            KEY_BACKSPACE, KEY_LEFT, KEY_RIGHT, KEY_HOME, KEY_END};
        for (auto key : keys) {
            auto func = text_input::hold_key_map_(key);
            if (IsKeyPressed(key)) {
                func(this);
                this->hold_key_start_ = 0.0;
                return;
            } else if (IsKeyDown(key)) {
                this->hold_key_start_ += GetFrameTime();
                if (this->hold_key_start_ >= INPUT_HOLD_KEY_START) {
                    func(this);
                    this->hold_key_start_ -= INPUT_HOLD_KEY_PERIOD;
                }
                return;
            }
        }
    }
}

void text_input::update_inputs_() {
    char* str = LoadUTF8(this->input_codepoints_.data(),
                         (int)this->input_codepoints_.size());
    this->input_text_ = str;
    UnloadUTF8(str);

    // Add all substring lengths of the text input so we can position the
    // cursor between each character when its moved.
    this->input_lengths_.resize(this->input_text_.size() + 1);
    for (int i = 0; i < this->input_text_.size(); i++) {
        Vector2 size = MeasureTextEx(GetFontDefault(),
                                     this->input_text_.substr(0, i).data(),
                                     FONT_SIZE,
                                     TEXT_SPACING);
        this->input_lengths_.at(i) = size.x;
    }

    // Last length, that's the full text input string.
    Vector2 size = MeasureTextEx(
        GetFontDefault(), this->input_text_.data(), FONT_SIZE, TEXT_SPACING);
    this->input_lengths_.at(this->input_text_.size()) = size.x;
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
        GetFontDefault(), label_render.data(), FONT_SIZE, TEXT_SPACING);
    DrawText(label_render.data(),
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
    Vector2 input_size = MeasureTextEx(
        GetFontDefault(), this->input_text_.data(), FONT_SIZE, TEXT_SPACING);
    DrawText(this->input_text_.data(),
             (int)this->rect.x + (int)label_size.x + INPUT_MARGIN * 3,
             (int)this->rect.y + (int)this->rect.height / 2 -
                 (int)input_size.y / 2,
             FONT_SIZE,
             input_col);

    // Render cursor
    if (this->focused_) {
        this->cursor_blink_time_ += GetFrameTime();
        if (this->cursor_blink_time_ >= INPUT_BLINK_TIME) {
            Vector2 cursor_top = (Vector2){
                0, this->rect.y + this->rect.height / 2 - input_size.y / 2};
            Vector2 cursor_bottom = (Vector2){
                0, this->rect.y + this->rect.height / 2 + input_size.y / 2};
            cursor_top.x = this->rect.x + label_size.x +
                           this->input_lengths_.at(this->cursor_pos_) +
                           INPUT_MARGIN * 3 + TEXT_SPACING;
            cursor_bottom.x = this->rect.x + label_size.x +
                              this->input_lengths_.at(this->cursor_pos_) +
                              INPUT_MARGIN * 3 + TEXT_SPACING;
            DrawLineEx(cursor_top, cursor_bottom, 2, input_col);
        }
        if (this->cursor_blink_time_ >= INPUT_BLINK_TIME * 2) {
            this->cursor_blink_time_ -= INPUT_BLINK_TIME * 2;
        }
    }

    // Set mouse cursor when being hovered
    if (this->is_being_hovered_()) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (GetMouseX() >=
            (int)this->rect.x + INPUT_MARGIN + (int)label_size.x) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        }
    };
}

void text_input::tick() {
    this->detect_clicks_();
    if (this->focused_) this->get_user_input_();
    this->render_();
}
