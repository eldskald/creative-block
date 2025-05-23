#include "engine/text.h"
#include "defs.h"
#include "imports.h"
#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include <string>

Font text::title_font_ = (Font){0};
Font text::label_font_ = (Font){0};
list<text*> text::texts_;

void text::initialize() {
    text::title_font_ = LoadFont(FONT_TITLE);
    text::label_font_ = GetFontDefault();
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
    for (auto text : text::texts_) {
        if (!text->hidden) {
            Vector2 size = MeasureTextEx(
                text->type == label ? text::label_font_ : text::title_font_,
                text->content.c_str(),
                text->type == label ? LABEL_FONT_SIZE : TITLE_FONT_SIZE,
                text->type == label ? LABEL_FONT_SPACING : TITLE_FONT_SPACING);
            Vector2 centered_pos = (Vector2){floor(text->pos.x - size.x / 2),
                                            floor(text->pos.y - size.y / 2)};
            DrawTextEx(
                text->type == label ? text::label_font_ : text::title_font_,
                text->content.c_str(),
                text->centered ? centered_pos : text->pos,
                text->type == label ? LABEL_FONT_SIZE : TITLE_FONT_SIZE,
                text->type == label ? LABEL_FONT_SPACING : TITLE_FONT_SPACING,
                MAIN_MASK_COLOR);
        }
    }
}
