#pragma once

#include <vector>
#include "render/command/cmd.h"
#include <atomic>
#include <thread>
#include <memory>
#include "render/command/command_pool.h"
#include <array>

namespace zidian{
    class IRender;
    class CommandPool;

    class CommandQueue{
    public:
        CommandQueue(const CommandQueue&) = delete;
        CommandQueue& operator=(const CommandQueue&) = delete;

        CommandQueue(IRender *render);

        ~CommandQueue();

        std::vector<CmdQueueType>& getWriteBuffer();

        std::vector<CmdQueueType>& getRenderBuffer();

        std::shared_ptr<CommandPool>& getCurrentCommandPool();

        void submitWriteBuffer();
    private:
        static const int QUEUE_COUNT = 2;

        std::vector<CmdQueueType> m_buffers[QUEUE_COUNT];
        std::array<std::shared_ptr<CommandPool>, QUEUE_COUNT> m_command_pools;

        std::atomic<int> m_front_index;
        std::atomic<int> m_back_index;
        std::atomic<bool> m_ready;
        
        IRender *m_render = nullptr;
    };
}

