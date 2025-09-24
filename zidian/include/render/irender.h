#pragma once

#include "glm/glm.hpp"

namespace zidian{
    class IRender{
    public:
        virtual int init() = 0;
        virtual int initEvironment() = 0;

        virtual ~IRender(){};           

        //------draw methods----
        virtual void drawPoint(float &x, float &y , glm::vec4 &color);

        virtual void setClearColor(glm::vec4 clear_color);

        virtual void clear();
    };
}

