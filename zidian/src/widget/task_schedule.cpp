#include "widget/task_schedule.h"
#include "utils/log.h"
#include "utils/time.h"

namespace zidian{
    std::atomic<int> TaskSchedule::taskid_index = 0;

    TaskSchedule::TaskSchedule(){
        m_thread_id = std::this_thread::get_id();
        Log::i("TaskSchedule", "create task schedule thread %lld", m_thread_id);
        m_time = CurrentTimeMillis();
    }

    TaskSchedule::~TaskSchedule(){
        Log::i("TaskSchedule", "destroy task schedule thread %lld", m_thread_id);
    }

     //delay 毫秒后 执行
    int TaskSchedule::schedule(std::function<void(void *)> runnable,long long delay){
        auto result_task = buildTimerTask(runnable , delay , TaskType::Once);
        m_task_list.push_back(result_task);
        return result_task->task_id;
    }

    int TaskSchedule::scheduleAtFixedRate(std::function<void(void *)> runnable ,long long period){
        auto resultTask = buildTimerTask(runnable , period , FixedRate);
        m_task_list.push_back(resultTask);
        return resultTask->task_id;
    }

    //移除延时任务
    bool TaskSchedule::removeScheduleTask(int remove_task_id){
        std::list<std::shared_ptr<Task>>::iterator removeIter;
        auto iter = m_task_list.begin();
        bool isDelete = false;
        
        while(iter != m_task_list.end()){
            std::shared_ptr<Task> task = *iter;
            if(task->task_id == remove_task_id){
                m_task_list.erase(iter);  
                isDelete = true;
                break;
            }
            iter++;
        }//end while
        return isDelete;
    }

    std::shared_ptr<Task> TaskSchedule::buildTimerTask(std::function<void(void *)> runnable,
        long long delay ,TaskType task_type){
        auto time_task = std::make_shared<Task>();

        time_task->task_id = genTaskId();
        time_task->should_runtime = CurrentTimeMillis() + delay;
        time_task->delay_time = delay;
        time_task->runnable = runnable;
        time_task->type = task_type;
        return time_task;
    }

    // 以固定时间 period 毫秒 执行
    //step a timestamp
    void TaskSchedule::tick(){
        m_time = CurrentTimeMillis();
        auto startTime = m_time;
        
        std::vector<std::list<std::shared_ptr<Task>>::iterator> remove_list;
        auto iter = m_task_list.begin();
        while(iter != m_task_list.end()){
            std::shared_ptr<Task> task = *iter;
            if(m_time >= task->should_runtime){
                task->runnable(nullptr);
                if(task->type == Once){
                    remove_list.push_back(iter);
                }else if(task->type == FixedRate){
                    task->should_runtime = CurrentTimeMillis() + task->delay_time;
                }
                // taskList_.erase(iter);
            }
            
            if(m_limit_tasks_runtime){
                auto endTime = CurrentTimeMillis();
                const auto costTime = endTime - startTime;
                // Log::i("timer", "task cost time : %ld", costTime);
                if(costTime >= 20){
                    break;
                }
            }
            iter++;
        }//end while

        if(!remove_list.empty()){
            for(auto iter : remove_list){
                m_task_list.erase(iter);
            }//end for each
        }
    }

    void TaskSchedule::setLimitTasksRumTime(bool is_limit){
        m_limit_tasks_runtime = is_limit;
    }
        
    void TaskSchedule::clear(){
        m_task_list.clear();
    }
}