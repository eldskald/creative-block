#pragma once
#include <raylib.h>

class cursor_manager {
public:
    static void tick();
    static void change_mouse_cursor(MouseCursor cursor);

private:
    static MouseCursor current_;
};
