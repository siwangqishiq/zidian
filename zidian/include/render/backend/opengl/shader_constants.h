#pragma once

#include <string>

namespace zidian{
    struct ShaderMeta{
        std::string name;
        std::string vertex_path;
        std::string fragment_path;
    };

    class ShaderMetas{
    public:
        static ShaderMeta DRAW_POINT;
    };
}
