#pragma once

namespace zidian{
    class IApp{
    public:
        virtual void onInit() = 0;
        virtual void onTick(float delta_time_micro) = 0;
        virtual void onDispose() = 0;
        virtual void onResize(int w, int h){}
    };
}