#pragma once

#include <string>
#include "utils/log.h"
#include "widget/app.h"
#include <thread>
#include <atomic>

class GLFWwindow;

namespace zidian{
    struct AppParams{
        std::string name;
    };

    
    class SandBox{
    public:
        void init(AppParams params);

        void setApp(IApp *app);

        int runLoop(int argc, char **argv);
    private:
        AppParams m_params;
        IApp *m_app = nullptr;

        std::thread m_render_thread;

        std::atomic<bool> is_exit = false;
        // bool is_exit = false;

        GLFWwindow *m_window = nullptr;
        
        void renderThreadFunc();
    };
}





