#include "player.h"
#include "base-unit.h"
#include "inputs.h"
#include <raylib.h>

void player::tick_() {
    this->time_ += GetFrameTime();

    this->change_dir(inputs::get_dir_input());

    if (inputs::is_action_pressed(inputs::action::jump)) this->press_jump();
    if (inputs::is_action_released(inputs::action::jump)) this->release_jump();

    this->base_unit::tick_();
}
