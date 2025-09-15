#pragma once

#include <string>

namespace zidian{
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





