#pragma once

#include <memory>

namespace zidian{
    class Cmd{
    public:
        Cmd();
        virtual ~Cmd();
        virtual void execute();
    };

    typedef std::shared_ptr<Cmd> CmdQueueType;
    // typedef Cmd* CmdQueueType;
}