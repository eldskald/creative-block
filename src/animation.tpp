#pragma once
#include "animation.h"
#include <raylib.h>
#include <tuple>

// Animations are a cycle of keyframe_ structs, each representing an animation
// keyframe that animates a variable given to the constructor by reference.
// You give a reference and tuples with the values it will take and the
// duration it should remain with that value.
//
// Each tuple will become a keyframe_ struct, with a pointer (next) pointing
// to the next keyframe_ struct and the last one points to the first if looped.
//
// When playing, the animation keeps track of time and when the current
// keyframe_'s duration expires, we change the animated value to the new one.

template <typename t>
animation<t>::animation(t* value,
                        bool looped,
                        tuple<t, float> frames[],
                        int size) {
    animation::keyframe_<t>* first_frame = nullptr;
    animation::keyframe_<t>* prev_frame = nullptr;
    for (int i = 0; i < size; ++i) {
        animation::keyframe_<t>* frame = new animation::keyframe_<t>;
        frame->value = get<t>(frames[i]);
        frame->duration = get<float>(frames[i]);
        if (i == 0) {
            prev_frame = frame;
            first_frame = frame;
        }
        prev_frame->next = frame;
        prev_frame = frame;
        if (i == size - 1) {
            frame->next = looped ? first_frame : nullptr;
        }
    }
    this->playing_ = false;
    this->looped_ = looped;
    this->value_ = value;
    this->curr_time_ = 0.0f;
    this->curr_frame_ = first_frame;
    this->first_frame_ = first_frame;
}

template <typename t> animation<t>::~animation() {
    animation::keyframe_<t>* next_frame = this->first_frame_->next;
    while (next_frame != this->first_frame_ && next_frame != nullptr) {
        animation::keyframe_<t>* next = next_frame->next;
        delete next_frame;
        next_frame = next;
    }
    delete this->first_frame_;
}

template <typename t> void animation<t>::play() {
    this->playing_ = true;
    *(this->value_) = this->curr_frame_->value;
}

template <typename t> void animation<t>::stop() {
    this->curr_frame_ = this->first_frame_;
    this->curr_time_ = 0.0f;
    this->playing_ = false;
}

template <typename t> void animation<t>::pause() {
    this->playing_ = false;
}

template <typename t> void animation<t>::tick_() {
    if (!(this->playing_)) return;
    this->curr_time_ += GetFrameTime();
    while (this->curr_time_ >= this->curr_frame_->duration) {
        this->curr_time_ -= this->curr_frame_->duration;
        this->curr_frame_ = this->curr_frame_->next;
        if (this->curr_frame_) {
            *(this->value_) = this->curr_frame_->value;
        } else {
            this->playing_ = false;
        }
    }
}
