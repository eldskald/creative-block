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
    static void reset_water_waves();
    static void set_color_shader(int index);

private:
    static Texture2D main_gradient_1_;
    static Texture2D shadow_gradient_1_;
    static Texture2D main_gradient_2_;
    static Texture2D shadow_gradient_2_;
    static RenderTexture2D base_tex_1_;
    static RenderTexture2D base_tex_2_;
    static RenderTexture2D colored_tex_;
    static RenderTexture2D blur_tex_1_;
    static RenderTexture2D blur_tex_2_;
    static RenderTexture2D final_tex_;
    static Vector2 stretched_tex_size_;
    static Vector2 window_size_;
    static vector<Shader> screen_color_shaders_;
    static Shader base_screen_shader_;
    static Shader blur_shader_1_;
    static Shader blur_shader_2_;
    static bool water_active_;
    static float water_level_;
    static vector<array<float, 4>> water_waves_;
    static float time_;
    static int current_screen_color_shader_;
#ifdef DEV
    static bool showing_areas_;
    static bool showing_fixed_bodies_;
    static bool showing_kinematic_bodies_;
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
    static void update_color_screen_();
};
