#pragma once

#include "render/command/cmd.h"
#include "glm/glm.hpp"

namespace zidian{
    class IRender;
    class CmdDrawPoint : public Cmd{
    public:
        CmdDrawPoint(IRender *render):Cmd(render){}

        void putParams(float x, float y, glm::vec4 color);

        virtual void execute() override;

        ~CmdDrawPoint();
    private:
        float m_x;
        float m_y;
        glm::vec4 m_color;
    };
}