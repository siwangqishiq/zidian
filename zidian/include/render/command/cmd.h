#pragma once

#include <memory>

namespace zidian{
    class IRender;
    class Cmd{
    public:
        Cmd(IRender *render):m_render(render){}
        
        virtual ~Cmd();
        virtual void execute();
    protected:
        IRender *m_render;
    };

    using CmdQueueType = std::shared_ptr<Cmd>;
    // typedef Cmd* CmdQueueType;
}