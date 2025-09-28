#pragma once

#include <memory>
#include "render/paint.h"

namespace zidian{
    class IRender;
    class Cmd{
    public:
        Paint m_paint;
        int m_type = 0;
        
        Cmd(int type):m_type(type){}
        Cmd():m_type(0){}

        virtual ~Cmd();
        virtual void execute();
    protected:
        // Render2d *m_render;
    };
    
    using CmdQueueType = std::shared_ptr<Cmd>;
}