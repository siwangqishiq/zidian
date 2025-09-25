#pragma once

#include <mutex>
#include <atomic>
#include <unordered_map>

namespace zidian{
    using RenderResHandle = int;

    class RenderResManager{
    public:
        RenderResManager();
        ~RenderResManager();

        RenderResHandle genResHandle();
    private:
        std::atomic<RenderResHandle> m_next_handle;
        std::mutex m_map_mutex;
    };
}

