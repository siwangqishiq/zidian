#pragma once

#include "render/render2d.h"
#include <string>

namespace zidian{
    struct AppParams{
        std::string name = "zidian_game";
        int view_width = 1280;
        int view_height = 800;
        bool full_screen = false;
        bool vsync = false;
        RenderBackend render_backend = RenderBackend::Opengl;
    };
    
    extern AppParams Config;
}