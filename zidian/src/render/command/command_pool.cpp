#include "utils/log.h"

#include "render/command/command_pool.h"
#include "render/command/types.h"

namespace zidian{
    CommandPool::CommandPool(IRender *render){
        m_render = render;
    }

    CommandPool::~CommandPool(){
    }

    void CommandPool::debugPrintInfo(){
        Log::i("command_pool", "command_pool ---- begin ------");
        for(auto &q : m_pool){
            Log::i("command_pool", "type:%d , size : %d , index : %d", 
                q.first, q.second.cmd_deque.size() , q.second.next_index);
        }//end for i
        Log::i("command_pool", "command_pool ---- end ------");
    }

    std::shared_ptr<Cmd> CommandPool::getCommandByType(int cmd_type){
        if(m_pool.find(cmd_type) != m_pool.end()){
            CmdDeque& cmdqueue = m_pool[cmd_type];
            if(cmdqueue.next_index < cmdqueue.cmd_deque.size()){
                auto cmd_ptr = cmdqueue.cmd_deque[cmdqueue.next_index];
                cmdqueue.next_index++;
                // Log::i("command_pool", "cache hitted");
                return cmd_ptr;
            }else{
                auto cmd_ptr = createPoolInstance(cmd_type);
                cmdqueue.cmd_deque.push_back(cmd_ptr);
                cmdqueue.next_index++;
                return cmd_ptr;
            }
        }else{
            CmdDeque cmd_queue;
            m_pool[cmd_type] = std::move(cmd_queue);
            auto cmd_ptr = createPoolInstance(cmd_type);
            m_pool[cmd_type].cmd_deque.push_back(cmd_ptr);
            m_pool[cmd_type].next_index++;
            return cmd_ptr;
        }
    }

    void CommandPool::resetPool(){
        for(auto &p : m_pool){
            p.second.next_index = 0;
            std::deque<std::shared_ptr<Cmd>> &cmd_deque = p.second.cmd_deque;
            while(cmd_deque.size() > m_limit_size){
                cmd_deque.pop_front();
            }//end while
        }//end for each
    }

    std::shared_ptr<Cmd> CommandPool::createPoolInstance(int cmd_type){
        if(m_render == nullptr){
            return std::make_shared<Cmd>(cmd_type);
        }
        return m_render->createCommandInstance(cmd_type);
    }
}


