#pragma once

#include "config.h"
#include "widget/app.h"
#include <thread>
#include <atomic>

class GLFWwindow;
class GLFWmonitor;

namespace zidian{
    class SandBox{
    public:
        void init(AppParams params);
        
        void setApp(IApp *app);

        int runLoop(int argc, char **argv);
    private:
        IApp *m_app = nullptr;

        std::thread m_render_thread;

        std::atomic<bool> is_exit = false;

        GLFWwindow *m_window = nullptr;
        GLFWmonitor *m_monitor = nullptr;

        //记录上一帧的时间
        double m_last_time_micro = 0.0f;
        
        double m_delta_time_micro = 0.0f;
        
        void updateTimeStamp();
        
        void mainThreadFunc();

        void renderThreadFunc();
    };
}

