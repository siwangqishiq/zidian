#pragma once

#include "render/command/cmd.h"
#include "glm/glm.hpp"
#include "render/paint.h"
#include "render/command/types.h"
#include "render/backend/opengl/glheader.h"

namespace zidian{
    class IRender;
    class CmdDrawPoint : public Cmd{
    public:
        CmdDrawPoint():Cmd(CMD_TYPE_DRAW_POINT){}

        void putParams(float x, float y, glm::vec4 color, Paint m_paint);

        virtual void execute() override;

        ~CmdDrawPoint();
    private:
        float m_x;
        float m_y;
        glm::vec4 m_color;
        Paint m_paint;

        GLuint m_draw_point_vao = 0;
        GLuint m_draw_point_buffer = 0;

        void prepareDrawPoint();
    };
}