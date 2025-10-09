#include "render/command_queue.h"
#include "utils/log.h"

namespace zidian{
    CommandQueue::CommandQueue(IRender *render){
        m_render = render;

        m_front_index.store(0);
        m_back_index.store(1);
        m_ready.store(false);

        for(int i = 0 ; i < QUEUE_COUNT; i++){
            m_command_pools[i] = std::make_shared<CommandPool>(m_render);
        }//end for i

        // for(int i = 0; i < QUEUE_COUNT ;i++){
        //     std::cout << "init command buffer size = " 
        //         << m_buffers[i].size() << std::endl;
        // }
    }

    CommandQueue::~CommandQueue(){
    }

    std::vector<CmdQueueType>& CommandQueue::getWriteBuffer(){
        while(m_ready.load()){
            std::this_thread::yield();
        }
        
        return m_buffers[m_back_index.load()];
    }

    std::shared_ptr<CommandPool>& CommandQueue::getCurrentCommandPool(){
        return m_command_pools[m_back_index.load()];
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

    void CommandQueue::showFrontBackIndex(){
        Log::i("command_queue", "front index :%d  back Index : %d", 
            m_front_index.load(),
            m_back_index.load()
        );
    }
}