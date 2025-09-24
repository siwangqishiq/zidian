#pragma once

#include "render/command/cmd.h"
#include "constants/types.h"

namespace zidian{
    class IRender;
    class CmdSetClearColor : public Cmd{
    public:
        CmdSetClearColor(IRender *render):Cmd(render){}

        void putParams(ColorType &color);
        
        virtual void execute() override;
    private:
        ColorType m_clear_color;
    };//end class
}