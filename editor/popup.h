#pragma once
#include "button.h"
#include <raylib.h>
#include <string>

using namespace std;

class popup {
public:
    string line_1{""};
    string line_2{""};
    bool confirm_button{false};
    void (*on_confirm)(){popup::empty_confirm_};

    void open();

    static void tick();
    static void close_current_popup();
    static bool is_popup_opened();

private:
    button confirm_btn_{button()};
    button close_btn_{button()};

    void update_btns_();
    void render_();

    static popup* opened_popup_;
    static popup* next_popup_;

    static void empty_confirm_();
};
