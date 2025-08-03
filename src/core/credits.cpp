#include "core/credits.h"
#include "core/scene-manager.h"
#include "defs.h"
#include "engine/sfx.h"

credits::credits() {
    auto* message = new text();
    message->content = CREDITS_SCREEN_MSG;
    message->centered = true;
    message->hidden = true;
    message->pos.x = WINDOW_SIZE_X / 2;
    message->pos.y = CREDITS_SCREEN_MSG_Y;
    this->add_child(message);
    this->message_ = message;
    auto* title = new text();
    title->content = GAME_TITLE;
    title->centered = true;
    title->type = text::title;
    title->pos.x = WINDOW_SIZE_X / 2;
    title->pos.y = CREDITS_SCREEN_TITLE_Y;
    this->add_child(title);
}

void credits::tick_() {
    this->time_ += GetFrameTime();
    if (this->time_ > CREDITS_SCREEN_MSG_CYCLE_TIME) {
        this->message_->hidden = !this->message_->hidden;
        this->time_ = 0.0f;
    }
    if (inputs::is_action_pressed(inputs::action::pause)) {
        sfx::play(sfx::select);
        scene_manager::next_scene();
    }
}
