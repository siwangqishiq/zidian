#pragma once

#include "constants/types.h"
#include "render/render2d.h"
#include <string>
#include "glm/glm.hpp"

namespace zidian{
    struct GlobalVars{
        int argc;
        char **argv;
    };
    
    struct AppParams{
        std::string name = "zidian_game";
        int view_width = 1280;
        int view_height = 800;
        bool full_screen = false;
        bool vsync = false;
        bool window_boardless = false;

        ColorType clear_color{0.0f, 0.0f, 0.0f, 1.0f};
        RenderBackend render_backend = RenderBackend::Opengl;
    };

    extern AppParams Config;
    extern GlobalVars Global;
}