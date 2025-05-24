#pragma once
#include <array>
#include <raylib.h>
#include <string>
#include <vector>

using namespace std;

class renderer {
public:
    static void initialize();
    static void unload();
    static void render();
    static void set_water(bool active, float water_level);
    static void add_water_wave(float origin, float amplitude, float width);

private:
    static RenderTexture2D base_tex_1_;
    static RenderTexture2D base_tex_2_;
    static RenderTexture2D blur_tex_1_;
    static RenderTexture2D blur_tex_2_;
    static RenderTexture2D final_tex_;
    static Vector2 stretched_tex_size_;
    static Vector2 window_size_;
    static Shader base_screen_shader_;
    static Shader blur_shader_1_;
    static Shader blur_shader_2_;
    static bool water_active_;
    static float water_level_;
    static vector<array<float, 4>> water_waves_;
#ifdef DEV
    static bool showing_areas_;
    static bool showing_fixed_bodies_;
    static bool showing_kinematic_bodies_;
    enum bloom_debug { base_1, base_2, blur_1, blur_2, final };
    static bloom_debug texture_rendered_;
#endif

    static void set_shader_property_(Shader shader,
                                     string property,
                                     const void* value,
                                     int uniform_type);
    static void stretch_texture_(Texture2D texture);
    static void update_tex_sizes_();
    static void update_window_size_();
    static void render_base_();
    static void post_process_();
    static void update_water_waves_();
};
