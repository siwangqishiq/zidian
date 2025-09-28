#pragma once

namespace zidian{
    class IRender;

    class Batch{
    public:
        Batch(IRender *render);
        virtual ~Batch();
        
        virtual void begin();
        virtual void end();
    protected:
        IRender *m_render;
    };
}