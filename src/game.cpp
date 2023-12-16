#include "game.h"
#include "game-element.h"
#include "sfx.h"
#include "sprite.h"
#include <raylib.h>

game_element* game::root_ = nullptr;

void game::tick_game_element_(game_element* element) {
    for (auto i = element->children_.begin(); i != element->children_.end();
         ++i) {
        tick_game_element_(*i);
    }
    element->tick_();
}

void game::update_game_element_pos_(game_element* element) {
    element->update_pos_();
    for (auto i = element->children_.begin(); i != element->children_.end();
         ++i) {
        update_game_element_pos_(*i);
    }
}

void game::initial_setup() {
    sprite::atlas_ = LoadTexture(SPRITESHEET_FILE);
    *(sprite::base_shader) = LoadShader("resources/shaders/base.vert",
                                        "resources/shaders/base.frag");
    sfx::sfx_1 = LoadWave(SFX_1);
    sfx::sfx_2 = LoadWave(SFX_2);
}

void game::do_game_loop() {
    if (!game::root_) return;
    game::update_game_element_pos_(game::root_);
    game::tick_game_element_(game::root_);
}

void game::set_root(game_element* new_root) {
    if (game::root_) {
        game::root_->trigger_exit_(game::root_);
    }
    game::root_ = new_root;
    game_element::trigger_enter_(new_root);
}

game_element* game::get_root() {
    return game::root_;
}
