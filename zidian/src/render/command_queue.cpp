#include "render/command_queue.h"
#include "utils/log.h"

namespace zidian{
    CommandQueue::CommandQueue(){
        m_front_index.store(0);
        m_back_index.store(1);
        m_ready.store(false);
    }

    CommandQueue::~CommandQueue(){
    }

    std::vector<CmdQueueType>& CommandQueue::getWriteBuffer(){
        while(m_ready.load(std::memory_order_acquire)){
            std::this_thread::yield();
        }
        
        return m_buffers[m_back_index.load(std::memory_order_acquire)];
    }

    std::vector<CmdQueueType>& CommandQueue::getRenderBuffer(){
        while(!m_ready.load(std::memory_order_acquire)){
            std::this_thread::yield();
        }

        m_ready.store(false, std::memory_order_release);
        return m_buffers[m_front_index.load(std::memory_order_acquire)];
    }

    void CommandQueue::submitWriteBuffer(){
        const int old_front = m_front_index.load(std::memory_order_relaxed);
        const int old_back = m_back_index.load(std::memory_order_relaxed);

        m_front_index.store(old_back, std::memory_order_release);
        m_back_index.store(old_front, std::memory_order_release);

        m_ready.store(true, std::memory_order_release);

        // Log::e("debug","m_front_index = %d, m_back_index = %d", 
        //     m_front_index.load(), m_back_index.load());
    }
}