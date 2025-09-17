#pragma once

namespace zidian{
    class IRender{
    public:
        virtual int initEnv() = 0;
        virtual ~IRender(){};
    };
}

