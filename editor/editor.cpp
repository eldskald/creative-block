#include "editor.h"
#include "defs.h"
#include "spritesheet.h"
#include "tilemap.h"
#include "tileset.h"
#include <raylib.h>
#include <vector>

using namespace std;

tilemap* editor::tilemap_ = nullptr;
tileset* editor::tileset_ = nullptr;

void editor::initialize() {
    SetExitKey(KEY_NULL);
    spritesheet::initialize();
    tileset::initialize();
    editor::tilemap_ = new tilemap();
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
                         FG_COLOR);

    editor::tilemap_->tick();
    editor::tileset_->tick();
}
