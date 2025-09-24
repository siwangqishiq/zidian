#include "render/command/cmd_set_clear_color.h"
#include "render/irender.h"

namespace zidian{
    void CmdSetClearColor::putParams(ColorType &color){
        m_clear_color = color;
    }

    void CmdSetClearColor::execute() {  
        m_render->setClearColor(m_clear_color);
    }
}