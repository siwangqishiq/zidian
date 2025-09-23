#pragma once

#include "render/command/cmd.h"
#include "glm/glm.hpp"

namespace zidian{
    class CmdDrawPoint : public Cmd{
    public:
        virtual void execute() override;

        CmdDrawPoint(float x,float y, glm::vec4 color);

        ~CmdDrawPoint();
    private:
        float m_x;
        float m_y;
        glm::vec4 m_color;
    };
}