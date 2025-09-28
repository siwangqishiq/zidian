#pragma once

#include <string>

namespace zidian{
    const std::string UNIFORM_NAME_SCRTONDC_MAT = "uScreenToNdcMat";
    const std::string UNIFORM_NAME_POINTSIZE = "uPointSize";

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
