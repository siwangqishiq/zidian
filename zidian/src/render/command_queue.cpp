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
        while(m_ready.load()){
            std::this_thread::yield();
        }
        
        return m_buffers[m_back_index.load()];
    }

    std::vector<CmdQueueType>& CommandQueue::getRenderBuffer(){
        while(!m_ready.load()){
            std::this_thread::yield();
        }

        m_ready.store(false);
        return m_buffers[m_front_index.load()];
    }

    void CommandQueue::submitWriteBuffer(){
        const int old_front = m_front_index.load();
        const int old_back = m_back_index.load();

        m_front_index.store(old_back);
        m_back_index.store(old_front);

        m_ready.store(true);
    }
}