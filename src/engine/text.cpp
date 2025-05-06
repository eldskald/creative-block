#include "text.h"
#include <raylib.h>
#include <string>

Font text::title_font_ = (Font){0};
Font text::label_font_ = (Font){0};
list<text*> text::texts_;

void text::initialize() {
    text::title_font_ = LoadFont(FONT_TITLE);
    text::label_font_ = LoadFont(FONT_LABEL);
}

void text::unload() {
    UnloadFont(text::title_font_);
    UnloadFont(text::label_font_);
}

void text::enter_() {
    text::texts_.push_back(this);
}

void text::exit_() {
    text::texts_.remove(this);
}

void text::render_texts_() {
    for (auto i : text::texts_) {
        DrawTextEx(i->type == label ? text::label_font_ : text::title_font_,
                   i->content.c_str(),
                   i->pos,
                   i->type == label ? LABEL_FONT_SIZE : TITLE_FONT_SIZE,
                   TEXT_SPACING,
                   MAIN_MASK_COLOR);
    }
}
