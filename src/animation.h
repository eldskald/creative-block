#pragma once
#include "game-element.h"
#include <tuple>

using namespace std;

template <typename t> class animation : public game_element {
public:
    animation(t* value, bool looped, tuple<t, float> frames[], int size);
    ~animation();

    void play();
    void stop();
    void pause();

private:
    template <typename u> struct keyframe_ {
        u value;
        float duration;
        keyframe_<u>* next;
    };

    t* value_;
    bool playing_;
    bool looped_;
    float curr_time_;
    keyframe_<t>* first_frame_;
    keyframe_<t>* curr_frame_;

    void tick_() override;
};

#include "animation.tpp"
