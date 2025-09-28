#pragma once

#include "render/command/cmd.h"
#include "render/command/types.h"


namespace zidian{
    class IRender;
    class CmdClear : public Cmd{
    public:
        CmdClear():Cmd(CMD_TYPE_CLEAR){}
        
        virtual void execute() override;
    };//end class
}