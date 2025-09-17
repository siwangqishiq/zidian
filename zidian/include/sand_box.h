#pragma once

#include "config.h"
#include "widget/app.h"
#include <thread>
#include <atomic>

class GLFWwindow;

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
        
        void renderThreadFunc();
        void mainThreadFunc();
    };
}

