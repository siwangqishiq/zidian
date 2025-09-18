#pragma once

#include "config.h"
#include "widget/app.h"
#include <thread>
#include <atomic>
#include <memory>
#include "widget/task_schedule.h"

class GLFWwindow;
class GLFWmonitor;

namespace zidian{
    class SandBox{
    public:
        void init(AppParams params);
        
        void setApp(IApp *app);

        int runLoop(int argc, char **argv);

        std::unique_ptr<TaskSchedule>& getMainTaskSchedule(){
            return m_main_task_schedule;
        }

        std::unique_ptr<TaskSchedule>& getRenderTaskSchedule(){
            return m_render_task_schedule;
        }

        int m_logic_fps = 0;
        int m_render_fps = 0;
    private:
        IApp *m_app = nullptr;

        std::thread m_render_thread;

        std::atomic<bool> is_exit = false;

        GLFWwindow *m_window = nullptr;
        GLFWmonitor *m_monitor = nullptr;

        //记录上一帧的时间
        double m_last_time_micro = 0.0f;
        
        double m_delta_time_micro = 0.0f;

        //主线程调度器 
        std::unique_ptr<TaskSchedule> m_main_task_schedule;
        std::unique_ptr<TaskSchedule> m_render_task_schedule;
        
        void updateTimeStamp(double &elapsed_time, int &fps_counter);
        
        void mainThreadFunc();

        void renderThreadFunc();
    };
}

