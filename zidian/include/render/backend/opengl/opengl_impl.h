#pragma once

#include "render/irender.h"

namespace zidian{
    class OpenglRender : public IRender{
        virtual int initEnv();

        virtual void drawPoint(float &x, float &y , glm::vec4 &color);
    };
}


