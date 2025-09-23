#pragma once

#include <vector>
#include "render/command/cmd.h"
#include <atomic>
#include <thread>
#include <memory>

namespace zidian{
    class CommandQueue{
    public:
        CommandQueue();

        ~CommandQueue();

        std::vector<CmdQueueType>& getWriteBuffer();

        std::vector<CmdQueueType>& getRenderBuffer();

        void submitWriteBuffer();
    private:
        static const int QUEUE_COUNT = 2;

        std::vector<CmdQueueType> m_buffers[QUEUE_COUNT];

        std::atomic<int> m_front_index;
        std::atomic<int> m_back_index;
        std::atomic<bool> m_ready;
    };
}

