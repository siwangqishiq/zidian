#include "render/command/cmd_draw_point.h"
#include "utils/log.h"

namespace zidian{
    CmdDrawPoint::CmdDrawPoint(float x,float y, glm::vec4 color){
        m_x = x;
        m_y = y;
        m_color = color;
    }

    CmdDrawPoint::~CmdDrawPoint(){
        // Log::i("cmd", "~CmdDrawPoint");
    }

    void CmdDrawPoint::execute() {  
        // Log::i("cmd", "CmdDrawPoint execute p : %f %f", m_x, m_y);
    }
}