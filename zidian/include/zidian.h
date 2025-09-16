#pragma once

#include <string>
#include "utils/log.h"

namespace zidian{
    bool UNITTEST = false;

    struct AppParams{
        std::string name;
    };

    class SandBox{
    public:
        void init(AppParams params);
        int runLoop(int argc, char **argv);
    private:
        AppParams m_params;
    };
}





