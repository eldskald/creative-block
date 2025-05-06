#pragma once
#include "game-element.h"
#include <raylib.h>
#include <list>
#include <string>

class text : public game_element {
    friend class renderer;

public:
    enum text_type { label, title };

    string content{""};
    text_type type{label};

    Vector2 get_size();

    static void initialize();
    static void unload();

protected:
    void enter_() override;
    void exit_() override;

private:
    static Font title_font_;
    static Font label_font_;
    static list<text*> texts_;

    static void render_texts_();
};
