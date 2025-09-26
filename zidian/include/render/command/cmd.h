#pragma once

#include <memory>
#include "render/paint.h"

namespace zidian{
    class IRender;
    class Cmd{
    public:
        Paint m_paint;
        int m_type = 0;
        
        Cmd(int type, IRender *render):m_render(render),m_type(type){}
        Cmd(IRender *render):m_render(render),m_type(0){}
        virtual ~Cmd();
        virtual void execute();
    protected:
        IRender *m_render;
    };

    using CmdQueueType = std::shared_ptr<Cmd>;
    // typedef Cmd* CmdQueueType;
}