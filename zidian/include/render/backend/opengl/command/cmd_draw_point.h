#pragma once

#include "glm/glm.hpp"

#include "render/paint.h"
#include "render/command/types.h"
#include "render/backend/opengl/command/cmd_base_opengl.h"

namespace zidian{
    class IRender;
    class CmdDrawPoint : public CmdBaseOpenGL{
    public:
        CmdDrawPoint():CmdBaseOpenGL(CMD_TYPE_DRAW_POINT){}

        void putParams(float x, float y, glm::vec4 color, Paint m_paint);

        virtual void execute() override;

        virtual ~CmdDrawPoint();
    private:
        glm::vec2 m_pos;
        glm::vec4 m_color;
        Paint m_paint;
        
        void prepareDrawPoint();
    };
}