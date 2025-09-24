#pragma once

#include "glm/glm.hpp"

namespace zidian{
    class IRender{
    public:
        virtual int initEnv() = 0;
        virtual ~IRender(){};
        
        virtual void drawPoint(float &x, float &y , glm::vec4 &color) = 0;
    };
}

