#pragma once
#include "engine/game-element.h"
#include <list>
#include <raylib.h>
#include <string>

class text : public game_element {
    friend class renderer;

public:
    enum text_type { label, title };

    string content{""};
    float hidden_for{0.0f};
    text_type type{label};
    bool centered{false};
    bool hidden{false};

    static void initialize();
    static void unload();

protected:
    void enter_() override;
    void exit_() override;
    void tick_() override;

private:
    static Font title_font_;
    static Font label_font_;
    static list<text*> texts_;

    static void render_texts_();
};
