#include "core/player.h"
#include "core/base-unit.h"
#include "core/death-particles.h"
#include "core/scene-manager.h"
#include "engine/inputs.h"
#include "engine/sprite.h"
#include <raylib.h>

player::player() {
    const float player_width = SPRITESHEET_CELL_SIZE_X;
    const float player_height = SPRITESHEET_CELL_SIZE_Y;
    const Vector2 player_sprite_coords = PLAYER_ATLAS_COORDS;
    this->collision_box = (Rectangle){0, 0, player_width, player_height};
    this->collision_mask = COL_MASK_PLAYER;
    this->collision_layer = COL_LAYER_PLAYER;
    auto* player_sprite = new sprite();
    player_sprite->atlas_coords = player_sprite_coords;
    player_sprite->tint = MAIN_MASK_COLOR;
    this->add_child(player_sprite);
    this->sprite_ = player_sprite;
    auto* emitter = new death_particles();
    emitter->tint = MAIN_MASK_COLOR;
    emitter->pos.y = -SPRITESHEET_CELL_SIZE_Y / 2;
    this->add_child(emitter);
    this->death_particles_emitter_ = emitter;
}

void player::kill() {
    if (this->killed_) return;
    this->killed_ = true;
    this->collision_layer = 0b00000000;
    this->collision_mask = 0b00000000;
    this->time_ = 0.0f;
    this->sprite_->mark_for_deletion();
    this->sprite_ = nullptr;
    this->death_particles_emitter_->emit();
}

void player::tick_() {
    this->time_ += GetFrameTime();
    if (this->killed_ && this->time_ >= PLAYER_RESPAWN_TIME) {
        this->mark_for_deletion();
        scene_manager::spawn_player();
    } else if (!this->killed_) {
        this->change_dir(inputs::get_dir_input());

        if (inputs::is_action_pressed(inputs::action::jump)) this->press_jump();
        if (inputs::is_action_released(inputs::action::jump))
            this->release_jump();

        this->base_unit::tick_();
    }
}
