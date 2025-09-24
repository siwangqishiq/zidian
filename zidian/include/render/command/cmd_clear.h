#pragma once

#include "render/command/cmd.h"

namespace zidian{
    class IRender;
    class CmdClear : public Cmd{
    public:
        CmdClear(IRender *render):Cmd(render){}
        
        virtual void execute() override;
    };//end class
}