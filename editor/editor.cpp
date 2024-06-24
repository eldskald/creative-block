#include "editor.h"
#include "button.h"
#include "data-exporter.h"
#include "defs.h"
#include "popup.h"
#include "spritesheet.h"
#include "text-input.h"
#include "tilemap.h"
#include "tileset_manager.h"
#include <raylib.h>

using namespace std;

bool editor::mouse_disabled = false;
tilemap* editor::tilemap_ = nullptr;
tileset_manager* editor::tileset_manager_ = nullptr;
button* blocks_btn = nullptr;
button* bg_btn = nullptr;
button* interact_btn = nullptr;

button* save_btn = nullptr;
button* load_btn = nullptr;
button* export_btn = nullptr;
text_input* file_input = nullptr;
button* open_test_popup_btn = nullptr;

void editor::save_tilemap_data() {
    if (file_input->get_input().empty()) return;
    string filepath =
        "editor/level-projects/" + file_input->get_input() + ".lvproj";
    string data = editor::tilemap_->convert_to_data();
    SaveFileText(filepath.c_str(), data.data());
}

void editor::load_tilemap_data() {
    if (file_input->get_input().empty()) return;
    string filepath =
        "editor/level-projects/" + file_input->get_input() + ".lvproj";
    char* data = LoadFileText(filepath.data());
    editor::tilemap_->load_from_data(data);
    UnloadFileText(data);
}

void editor::export_tilemap_data() {
    if (file_input->get_input().empty()) return;
    string data = data_exporter::get_export_text(editor::tilemap_->get_cells());
    string filepath = "scenes/" + file_input->get_input() + ".dat";
    SaveFileText(filepath.c_str(), data.data());
}

void editor::change_to_blocks_tileset() {
    tileset_manager::selected_set = tileset::blocks;
    blocks_btn->set_toggle(true);
    bg_btn->set_toggle(false);
    interact_btn->set_toggle(false);
}

void editor::change_to_bg_tileset() {
    tileset_manager::selected_set = tileset::background;
    blocks_btn->set_toggle(false);
    bg_btn->set_toggle(true);
    interact_btn->set_toggle(false);
}

void editor::change_to_interact_tileset() {
    tileset_manager::selected_set = tileset::interact;
    blocks_btn->set_toggle(false);
    bg_btn->set_toggle(false);
    interact_btn->set_toggle(true);
}

void editor::initialize() {
    SetExitKey(KEY_NULL);
    spritesheet::initialize();
    tileset_manager::initialize();
    editor::tilemap_ = new tilemap();

    blocks_btn = new button();
    blocks_btn->label = "blocks";
    blocks_btn->toggle_mode = true;
    blocks_btn->rect = (Rectangle){4, TILESET_ORIGIN_Y - 40, 132, 32}; // NOLINT
    blocks_btn->on_click = editor::change_to_blocks_tileset;
    blocks_btn->set_toggle(true);

    bg_btn = new button();
    bg_btn->label = "background";
    bg_btn->toggle_mode = true;
    bg_btn->rect = (Rectangle){152, TILESET_ORIGIN_Y - 40, 132, 32}; // NOLINT
    bg_btn->on_click = editor::change_to_bg_tileset;

    interact_btn = new button();
    interact_btn->label = "interact";
    interact_btn->toggle_mode = true;
    interact_btn->rect =
        (Rectangle){300, TILESET_ORIGIN_Y - 40, 132, 32}; // NOLINT
    interact_btn->on_click = editor::change_to_interact_tileset;

    save_btn = new button();
    save_btn->label = "save";
    save_btn->rect =
        (Rectangle){16, EDITOR_WINDOW_SIZE_Y - 40, 100, 32}; // NOLINT
    save_btn->on_click = editor::save_tilemap_data;

    load_btn = new button();
    load_btn->label = "load";
    load_btn->rect =
        (Rectangle){132, EDITOR_WINDOW_SIZE_Y - 40, 100, 32}; // NOLINT
    load_btn->on_click = editor::load_tilemap_data;

    export_btn = new button();
    export_btn->label = "export";
    export_btn->rect =
        (Rectangle){248, EDITOR_WINDOW_SIZE_Y - 40, 100, 32}; // NOLINT
    export_btn->on_click = editor::export_tilemap_data;

    file_input = new text_input();
    file_input->label = "level name";
    file_input->rect =
        (Rectangle){364, EDITOR_WINDOW_SIZE_Y - 40, 400, 32}; // NOLINT
}

void editor::tick() {
    ClearBackground(BG_COLOR);

    // Reset mouse cursor
    SetMouseCursor(MOUSE_CURSOR_ARROW);

    // Draw the blue rectangle around the tilemap
    DrawRectangleLinesEx((Rectangle){TILEMAP_ORIGIN_X - 1,
                                     TILEMAP_ORIGIN_Y - 1,
                                     TILEMAP_PIXEL_SIZE_X + 2,
                                     TILEMAP_PIXEL_SIZE_Y + 2},
                         1,
                         PRIMARY_COLOR);

    // Popups disable the UI
    editor::mouse_disabled = popup::is_popup_opened();

    editor::tilemap_->tick();
    editor::tileset_manager_->tick();
    blocks_btn->tick();
    bg_btn->tick();
    interact_btn->tick();
    save_btn->tick();
    load_btn->tick();
    export_btn->tick();
    file_input->tick();

    // Popups ignore mouse disabled feature because they themselves disable it
    if (editor::mouse_disabled) {
        editor::mouse_disabled = false;
        popup::tick();
        editor::mouse_disabled = true;
    } else {
        popup::tick();
    }
}
