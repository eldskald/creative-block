#include "editor.h"
#include "button.h"
#include "data-exporter.h"
#include "defs.h"
#include "popup.h"
#include "spritesheet.h"
#include "text-input.h"
#include "tilemap.h"
#include "tileset.h"
#include <iostream>
#include <raylib.h>
#include <vector>

using namespace std;

bool editor::mouse_disabled = false;
tilemap* editor::tilemap_ = nullptr;
tileset* editor::tileset_ = nullptr;

button* save_btn = nullptr;
button* load_btn = nullptr;
button* export_btn = nullptr;
text_input* file_input = nullptr;
popup* test_popup = nullptr;
button* open_test_popup_btn = nullptr;

void open_test_popup() {
    test_popup->open();
}

void confirm_test_popup() {
    cout << "funcionou weeee" << endl;
    popup::close_current_popup();
}

void editor::save_tilemap_data() {
    if (file_input->get_input().empty()) return;
    string filepath =
        "editor/level-projects/" + file_input->get_input() + ".lvproj";
    string data = editor::tilemap_->convert_to_data();
    SaveFileText(filepath.data(), data.data());
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
    string filepath = "levels/" + file_input->get_input() + ".dat";
    SaveFileText(filepath.data(), data.data());
}

void editor::initialize() {
    SetExitKey(KEY_NULL);
    spritesheet::initialize();
    tileset::initialize();
    editor::tilemap_ = new tilemap();

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

    open_test_popup_btn = new button();
    open_test_popup_btn->label = "popup";
    open_test_popup_btn->rect =
        (Rectangle){800, EDITOR_WINDOW_SIZE_Y - 40, 100, 32}; // NOLINT
    open_test_popup_btn->on_click = open_test_popup;

    test_popup = new popup();
    test_popup->line_1 = "reeeeeeeeeee!!";
    test_popup->line_2 = "teste som oi teste";
    test_popup->confirm_button = true;
    test_popup->on_confirm = confirm_test_popup;
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
    editor::tileset_->tick();
    save_btn->tick();
    load_btn->tick();
    export_btn->tick();
    file_input->tick();
    open_test_popup_btn->tick();

    // Popups ignore mouse disabled feature because they themselves disable it
    if (editor::mouse_disabled) {
        editor::mouse_disabled = false;
        popup::tick();
        editor::mouse_disabled = true;
    } else {
        popup::tick();
    }
}
