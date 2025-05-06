#include "cursor-manager.h"
#include <raylib.h>

MouseCursor cursor_manager::current_ = MOUSE_CURSOR_DEFAULT;

void cursor_manager::tick() {
    SetMouseCursor(cursor_manager::current_);
    cursor_manager::current_ = MOUSE_CURSOR_DEFAULT;
}

void cursor_manager::change_mouse_cursor(MouseCursor cursor) {
    cursor_manager::current_ = cursor;
}
