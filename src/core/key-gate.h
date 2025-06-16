#pragma once
#include "core/key-particles.h"
#include "engine/physics-body.h"
#include "engine/sprite.h"
#include <list>
#include <raylib.h>

class renderer;

class key_gate : public physics_body {
    friend class renderer;

public:
    float height{0.0f};
    list<Vector2> key_positions;

    key_gate();

protected:
    void enter_() override;
    void exit_() override;
    void tick_() override;
    void on_player_respawn_() override;
    void on_player_shadow_() override;

private:
    class key_ : public physics_body {
    public:
        key_(key_gate* gate);

        bool is_active();

    protected:
        void body_entered_(physics_body* body) override;
        void on_player_respawn_() override;
        void on_player_shadow_() override;

    private:
        key_gate* gate_{nullptr};
        sprite* key_sprite_{nullptr};
        key_particles* particles_{nullptr};
        bool active_{true};

        void enable_();
    };

    list<key_*> keys_;
    physics_body* gate_collider_{nullptr};
    float current_height_{0.0f};
    bool active_{true};

    void key_gotten_();
    void enable_();

    static list<key_gate*> key_gates_;

    static void render_gates_();
};
