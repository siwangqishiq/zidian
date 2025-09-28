#pragma once

#include "render/backend/opengl/command/cmd_base_opengl.h"
#include "render/command/types.h"

namespace zidian{
    class IRender;
    class CmdClear : public CmdBaseOpenGL{
    public:
        CmdClear():CmdBaseOpenGL(CMD_TYPE_CLEAR){}
        
        virtual void execute() override;
    };//end class
}