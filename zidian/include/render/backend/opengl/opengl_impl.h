#pragma once

#include "render/irender.h"

namespace zidian{
    class OpenglRender : public IRender{
        virtual int init() override;

        virtual int initEvironment() override;

        virtual void clear() override;
        virtual void setClearColor(glm::vec4 clear_color) override;

        virtual void drawPoint(float &x, float &y , glm::vec4 &color);
    };
}


