#include "render/backend/opengl/command/cmd_set_clear_color.h"
#include "render/irender.h"
#include "utils/log.h"

namespace zidian{
    void CmdSetClearColor::putParams(ColorType &color){
        m_clear_color = color;
    }

    CmdSetClearColor::~CmdSetClearColor(){
    }

    void CmdSetClearColor::execute() {  
        // Log::blue_log("debug" , "CmdSetClearColor run execute gl %f %f %f %f",
        //     m_clear_color[0],
        //     m_clear_color[1],
        //     m_clear_color[2],
        //     m_clear_color[3]);
        
        glClearColor(m_clear_color[0], 
            m_clear_color[1] ,
            m_clear_color[2] ,
            m_clear_color[3]);

        // std::cout << glGetError() << std::endl;
    }
}