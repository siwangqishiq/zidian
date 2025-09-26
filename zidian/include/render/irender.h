#pragma once

#include "glm/glm.hpp"
#include "config.h"
#include "render/paint.h"

namespace zidian{
    class IRender{
    public:
        virtual int init() = 0;
        virtual int initEvironment() = 0;

        virtual void dispose();

        virtual void onSizeChanged(int view_width, int view_height);

        virtual ~IRender(){};           

        //------draw methods----
        virtual void drawPoint(float &x, float &y , glm::vec4 &color, Paint &paint);

        virtual void setClearColor(glm::vec4 clear_color);

        virtual void clear();
    };
}

