#pragma once

#include "render/command/cmd.h"
#include "constants/types.h"
#include "render/command/types.h"
#include "render/backend/opengl/glheader.h"

namespace zidian{
    class IRender;
    class CmdSetClearColor : public Cmd{
    public:
        CmdSetClearColor():Cmd(CMD_TYPE_SET_CLEAR_COLOR){}

        void putParams(ColorType &color);
        
        virtual void execute() override;

        virtual ~CmdSetClearColor();
    private:
        ColorType m_clear_color;
    };//end class
}