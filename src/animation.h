#pragma once
#include "game-element.h"
#include <tuple>
#include <vector>

using namespace std;

template <typename t> class animation : public game_element {
public:
    animation(const animation&) = delete;
    animation(animation&&) = delete;
    animation& operator=(const animation&) = delete;
    animation& operator=(animation&&) = delete;
    animation(t* value, bool looped, vector<tuple<t, float>> frames);
    ~animation() override;

    void play();
    void stop();
    void pause();

private:
    template <typename u> struct keyframe_ {
        u value;
        float duration;
        keyframe_<u>* next;
    };

    t* value_{nullptr};
    bool playing_{false};
    bool looped_{false};
    float curr_time_{0.0f};
    keyframe_<t>* first_frame_{nullptr};
    keyframe_<t>* curr_frame_{nullptr};

    void tick_() override;
};

#include "animation.tpp"
