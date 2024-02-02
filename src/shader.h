#pragma once
#include <raylib.h>

class shader {
public:
    static void initialize();
    static Shader* get_base();
    static Shader* get_sample();

private:
    static Shader base_;
    static Shader sample_;
};
