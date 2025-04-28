#pragma once
#include <raylib.h>
#include <string>

using namespace std;

class renderer {
public:
    static void initialize();
    static void render();

private:
    static RenderTexture2D base_tex_1_;
    static RenderTexture2D base_tex_2_;
    static RenderTexture2D blur_tex_1_;
    static RenderTexture2D blur_tex_2_;
    static RenderTexture2D final_tex_;
    static Vector2 stretched_tex_size_;
    static Vector2 window_size_;
    static Shader base_screen_shader_;
    static Shader blur_shader_;
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
};
