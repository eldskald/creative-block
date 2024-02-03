#include "editor.h"
#include "defs.h"
#include "tilemap.h"
#include <raylib.h>

tilemap* editor::tilemap_ = new tilemap();

void editor::initialize() {
    SetExitKey(KEY_NULL);
    defs::initialize();
}

void editor::tick() {
    ClearBackground(EDITOR_BG_COLOR);

    // Account for the border around the tilemap
    DrawRectangleLinesEx((Rectangle){TILEMAP_ORIGIN_X - 1,
                                     TILEMAP_ORIGIN_Y - 1,
                                     TILEMAP_PIXEL_SIZE_X + 2,
                                     TILEMAP_PIXEL_SIZE_Y + 2},
                         1,
                         BLUE);

    editor::tilemap_->tick();
}
