#include "editor.h"
#include "button.h"
#include "data-exporter.h"
#include "defs.h"
#include "spritesheet.h"
#include "text-input.h"
#include "tilemap.h"
#include "tileset.h"
#include <raylib.h>
#include <vector>

using namespace std;

tilemap* editor::tilemap_ = nullptr;
tileset* editor::tileset_ = nullptr;

button* save_btn = nullptr;
button* load_btn = nullptr;
button* export_btn = nullptr;
text_input* filepath_input = nullptr;

void editor::save_tilemap_data() {
    string data = editor::tilemap_->convert_to_data();
    SaveFileText("testmap.lvproj", data.data());
}

void editor::load_tilemap_data() {
    char* data = LoadFileText("testmap.lvproj");
    editor::tilemap_->load_from_data(data);
    UnloadFileText(data);
}

void editor::export_tilemap_data() {
    string data = data_exporter::get_export_text(editor::tilemap_->get_cells());
    SaveFileText("testlevel.dat", data.data());
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
        (Rectangle){900, EDITOR_WINDOW_SIZE_Y - 40, 100, 32}; // NOLINT
    export_btn->on_click = editor::export_tilemap_data;

    filepath_input = new text_input();
    filepath_input->label = "file path";
    filepath_input->rect =
        (Rectangle){248, EDITOR_WINDOW_SIZE_Y - 40, 400, 32}; // NOLINT
}

void editor::tick() {
    ClearBackground(BG_COLOR);

    // Reset mouse cursor.
    SetMouseCursor(MOUSE_CURSOR_ARROW);

    // Draw the blue rectangle around the tilemap.
    DrawRectangleLinesEx((Rectangle){TILEMAP_ORIGIN_X - 1,
                                     TILEMAP_ORIGIN_Y - 1,
                                     TILEMAP_PIXEL_SIZE_X + 2,
                                     TILEMAP_PIXEL_SIZE_Y + 2},
                         1,
                         PRIMARY_COLOR);

    editor::tilemap_->tick();
    editor::tileset_->tick();
    save_btn->tick();
    load_btn->tick();
    export_btn->tick();
    filepath_input->tick();
}
