#pragma once

#include <functional>
#include <list>
#include <memory>
#include <thread>
#include <atomic>

namespace zidian{
    enum TaskType{
        Once, //仅运行一次
        FixedRate //以固定频率执行
    };

    struct Task{
        int task_id;
        long long should_runtime = 0;
        long long delay_time = 0;
        std::function<void(void *)> runnable;
        TaskType type = Once;
    };

    class TaskSchedule{
    public:
        TaskSchedule();
        ~TaskSchedule();

         //delay 毫秒后 执行
        int schedule(std::function<void(void *)> runnable, long long delay);
        
        // 以固定时间 period 毫秒 执行
        int scheduleAtFixedRate(std::function<void(void *)> runnable ,long long period);

        //移除定时任务
        bool removeScheduleTask(int taskId);

        //step a timestamp
        void tick();
        
        void clear();

        void setLimitTasksRumTime(bool is_limit);
     private:
        static std::atomic<int> taskid_index;

        std::thread::id m_thread_id;
        long long m_time;
        bool m_limit_tasks_runtime = true;//是否限制任务列表在每一帧的运行时间 默认限制

        std::list<std::shared_ptr<Task>> m_task_list;
        
        static int genTaskId(){
            return taskid_index++;
        }
        
        std::shared_ptr<Task> buildTimerTask(std::function<void(void *)> runnable,
            long long delay ,TaskType task_type);
    };
}