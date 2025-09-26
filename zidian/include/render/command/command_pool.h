#pragma once

#include <memory>
#include <deque>
#include "render/command/cmd.h"
#include <unordered_map>
#include "render/irender.h"

namespace zidian{
    struct CmdDeque{
        std::deque<std::shared_ptr<Cmd>> cmd_deque;
        int next_index = 0;
    };

    class CommandPool{
    public:
        CommandPool(IRender *render);

        std::shared_ptr<Cmd> getCommandByType(int cmd_type);

        void resetPool();

        ~CommandPool();

        std::shared_ptr<Cmd> createPoolInstance(int cmd_type);

        void debugPrintInfo();
    private:
        int m_limit_size = 1024;
        std::unordered_map<int, CmdDeque> m_pool;

        IRender *m_render;
    };

}