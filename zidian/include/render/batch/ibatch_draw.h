#pragma once

namespace zidian{
    class IBatchDraw{
    public:
        virtual void begin() = 0;
        virtual void end() = 0;
    };
}