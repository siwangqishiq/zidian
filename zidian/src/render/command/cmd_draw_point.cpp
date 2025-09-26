#include "render/command/cmd_draw_point.h"
#include "utils/log.h"
#include "render/irender.h"

namespace zidian{
    void CmdDrawPoint::putParams(float x, float y, glm::vec4 color){
        m_x = x;
        m_y = y;
        m_color = color;
    }

    CmdDrawPoint::~CmdDrawPoint(){
    }

    void CmdDrawPoint::execute() {
        m_render->drawPoint(m_x, m_y, m_color, m_paint);
    }
}