#include "editor.h"
#include "button.h"
#include "cursor-manager.h"
#include "data-exporter.h"
#include "defs.h"
#include "popup.h"
#include "spritesheet.h"
#include "text-input.h"
#include "tilemap.h"
#include "tileset-manager.h"
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
button* clear_btn = nullptr;
text_input* shadows_input = nullptr;
text_input* screen_colors_input = nullptr;
text_input* file_input = nullptr;
text_input* txt_1_input = nullptr;
text_input* txt_2_input = nullptr;
text_input* txt_3_input = nullptr;
text_input* txt_1_hf_input = nullptr;
text_input* txt_2_hf_input = nullptr;
text_input* txt_3_hf_input = nullptr;
button* open_test_popup_btn = nullptr;

void editor::save_tilemap_data() {
    if (file_input->get_input().empty()) return;
    string filepath =
        "editor/level-projects/" + file_input->get_input() + ".lvproj";
    string data = editor::tilemap_->convert_to_data();

    data += shadows_input->get_input() + PROJ_TEXT_SEPARATOR;
    data += txt_1_input->get_input() + PROJ_TEXT_SEPARATOR;
    data += txt_2_input->get_input() + PROJ_TEXT_SEPARATOR;
    data += txt_3_input->get_input() + PROJ_TEXT_SEPARATOR;
    data += txt_1_hf_input->get_input() + PROJ_TEXT_SEPARATOR;
    data += txt_2_hf_input->get_input() + PROJ_TEXT_SEPARATOR;
    data += txt_3_hf_input->get_input() + PROJ_TEXT_SEPARATOR;
    data += screen_colors_input->get_input();

    SaveFileText(filepath.c_str(), data.data());
}

void editor::load_tilemap_data() {
    if (file_input->get_input().empty()) return;
    string filepath =
        "editor/level-projects/" + file_input->get_input() + ".lvproj";
    char* data = LoadFileText(filepath.data());
    editor::tilemap_->load_from_data(data);

    string texts = ((string)data).substr(TILEMAP_SIZE_X * TILEMAP_SIZE_Y * 3);
    int first = -1;
    int sec = -1;
    int third = -1;
    int fourth = -1;
    int fifth = -1;
    int sixth = -1;
    int seventh = -1;

    for (int i = 0; i < (int)texts.length(); i++) {
        if (texts.at(i) == PROJ_TEXT_SEPARATOR) {
            if (first == -1) {
                first = i;
                continue;
            }
            if (first != -1 && sec == -1) {
                sec = i;
                continue;
            }
            if (first != -1 && sec != -1 && third == -1) {
                third = i;
                continue;
            }
            if (first != -1 && sec != -1 && third != -1 && fourth == -1) {
                fourth = i;
                continue;
            }
            if (first != -1 && sec != -1 && third != -1 && fourth != -1 &&
                fifth == -1) {
                fifth = i;
                continue;
            }
            if (first != -1 && sec != -1 && third != -1 && fourth != -1 &&
                fifth != -1 && sixth == -1) {
                sixth = i;
                continue;
            }
            if (first != -1 && sec != -1 && third != -1 && fourth != -1 &&
                fifth != -1 && sixth != -1 && seventh == -1) {
                seventh = i;
                continue;
            }
        }
    }
    shadows_input->set_input(texts.substr(0, first));
    txt_1_input->set_input(texts.substr(first + 1, sec - first - 1));
    txt_2_input->set_input(texts.substr(sec + 1, third - sec - 1));
    txt_3_input->set_input(texts.substr(third + 1, fourth - third - 1));
    txt_1_hf_input->set_input(texts.substr(fourth + 1, fifth - fourth - 1));
    txt_2_hf_input->set_input(texts.substr(fifth + 1, sixth - fifth - 1));
    txt_3_hf_input->set_input(texts.substr(sixth + 1, seventh - sixth - 1));
    screen_colors_input->set_input(texts.substr(seventh + 1));

    UnloadFileText(data);
}

void editor::export_tilemap_data() {
    if (file_input->get_input().empty()) return;
    string data = data_exporter::get_export_text(editor::tilemap_->get_cells(),
                                                 txt_1_input->get_input(),
                                                 txt_1_hf_input->get_input(),
                                                 txt_2_input->get_input(),
                                                 txt_2_hf_input->get_input(),
                                                 txt_3_input->get_input(),
                                                 txt_3_hf_input->get_input(),
                                                 shadows_input->get_input(),
                                                 screen_colors_input->get_input());
    string filepath = "assets/scenes/" + file_input->get_input() + ".dat";
    SaveFileText(filepath.c_str(), data.data());
}

void editor::clear_tilemap() {
    tilemap_->clear();
    shadows_input->set_input("");
    txt_1_input->set_input("");
    txt_1_hf_input->set_input("");
    txt_2_input->set_input("");
    txt_2_hf_input->set_input("");
    txt_3_input->set_input("");
    txt_3_hf_input->set_input("");
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

    clear_btn = new button();
    clear_btn->label = "clear";
    clear_btn->rect =
        (Rectangle){364, EDITOR_WINDOW_SIZE_Y - 40, 100, 32}; // NOLINT
    clear_btn->on_click = editor::clear_tilemap;

    file_input = new text_input();
    file_input->label = "level name";
    file_input->rect =
        (Rectangle){480, EDITOR_WINDOW_SIZE_Y - 40, 300, 32}; // NOLINT

    shadows_input = new text_input();
    shadows_input->label = "level shadows";
    shadows_input->number_input = true;
    shadows_input->rect =
        (Rectangle){796, EDITOR_WINDOW_SIZE_Y - 40, 200, 32}; // NOLINT

    screen_colors_input = new text_input();
    screen_colors_input->label = "level colors";
    screen_colors_input->number_input = true;
    screen_colors_input->rect =
        (Rectangle){1012, EDITOR_WINDOW_SIZE_Y - 40, 200, 32}; // NOLINT

    txt_1_input = new text_input();
    txt_1_input->label = "level text 1";
    txt_1_input->rect = (Rectangle){624, TILESET_ORIGIN_Y, 600, 32}; // NOLINT

    txt_2_input = new text_input();
    txt_2_input->label = "level text 2";
    txt_2_input->rect =
        (Rectangle){624, TILESET_ORIGIN_Y + 40, 600, 32}; // NOLINT

    txt_3_input = new text_input();
    txt_3_input->label = "level text 3";
    txt_3_input->rect =
        (Rectangle){624, TILESET_ORIGIN_Y + 80, 600, 32}; // NOLINT

    txt_1_hf_input = new text_input();
    txt_1_hf_input->label = "text 1 hf";
    txt_1_hf_input->number_input = true;
    txt_1_hf_input->rect =
        (Rectangle){624, TILESET_ORIGIN_Y - 40, 160, 32}; // NOLINT

    txt_2_hf_input = new text_input();
    txt_2_hf_input->label = "text 2 hf";
    txt_2_hf_input->number_input = true;
    txt_2_hf_input->rect =
        (Rectangle){800, TILESET_ORIGIN_Y - 40, 160, 32}; // NOLINT

    txt_3_hf_input = new text_input();
    txt_3_hf_input->label = "text 3 hf";
    txt_3_hf_input->number_input = true;
    txt_3_hf_input->rect =
        (Rectangle){976, TILESET_ORIGIN_Y - 40, 160, 32}; // NOLINT
}

void editor::tick() {
    ClearBackground(BG_COLOR);

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
    clear_btn->tick();
    file_input->tick();
    shadows_input->tick();
    screen_colors_input->tick();
    txt_1_input->tick();
    txt_2_input->tick();
    txt_3_input->tick();
    txt_1_hf_input->tick();
    txt_2_hf_input->tick();
    txt_3_hf_input->tick();

    // Popups ignore mouse disabled feature because they themselves disable it
    if (editor::mouse_disabled) {
        editor::mouse_disabled = false;
        popup::tick();
        editor::mouse_disabled = true;
    } else {
        popup::tick();
    }

    // Sets the mouse cursor
    cursor_manager::tick();
}
