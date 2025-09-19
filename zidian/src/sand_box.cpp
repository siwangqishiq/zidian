#include "zidian.h"
#include <iostream>

#ifndef __ANDROID__
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif

namespace zidian{
    const std::string TAG = "zidian";

    void SandBox::init(AppParams params){
        Config = params;
    }

    void SandBox::setApp(IApp *app){
        m_app = app;
    }

    int SandBox::runLoop(int argc, char **argv){
        Global.argc = argc;
        Global.argv = argv;
        
        Log::w(TAG,"start main thread: %ld", std::this_thread::get_id());
        
        glfwInit();

        if(Config.full_screen){
            m_monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* vm = glfwGetVideoMode(m_monitor);
            Config.view_width = vm->width;
            Config.view_height = vm->height;
            Log::i(TAG, "full set screen size : %d x %d", Config.view_width, Config.view_height);
        }

        m_window = glfwCreateWindow(Config.view_width, Config.view_height, 
            Config.name.c_str(), m_monitor, nullptr);

        m_render_thread = std::thread([this](){
           renderThreadFunc();
        });

        mainThreadFunc();

        this->is_exit = true;
        m_render_thread.join();
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return 0;
    }

    void SandBox::mainThreadFunc(){
        m_main_task_schedule = std::make_unique<TaskSchedule>();

        if(m_app != nullptr){
            m_app->onInit();
        }

        m_last_time_micro = CurrentTimeMircoDoubleFloat();
        int fps_counter = 0;
        double elapsed_time = 0.0f;

        while(!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
            if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                this->is_exit = true;
                break;
            }
            updateTimeStamp(elapsed_time , fps_counter);

            if(m_app != nullptr){
                m_app->onTick(m_delta_time_micro);
            }
            
            if(m_main_task_schedule != nullptr){
                m_main_task_schedule->tick();
            }

        }//end while

        if(m_main_task_schedule != nullptr){
            m_main_task_schedule->clear();
        }

        if(m_app != nullptr){
            m_app->onDispose();
        }
    }

    void SandBox::updateTimeStamp(double &elapsed_time, int &fps_counter){
        double cur_time = CurrentTimeMircoDoubleFloat();
        m_delta_time_micro = cur_time - m_last_time_micro;
        m_last_time_micro = cur_time;

        elapsed_time += m_delta_time_micro;
        if(elapsed_time >= 1000000.0f){
            m_logic_fps = fps_counter;
            elapsed_time = 0.0f;
            fps_counter = 0;
        }else{
            fps_counter++;
        }
    }

    void SandBox::renderThreadFunc(){
        Log::w(TAG,"start render thread: %ld", std::this_thread::get_id());

        glfwMakeContextCurrent(m_window);
        m_render_task_schedule = std::make_unique<TaskSchedule>();
        Render2d::getInstance();

        double elapsed_time = 0.0f;
        double last_time = CurrentTimeMircoDoubleFloat();
        int fps_counter = 0;

        glfwSwapInterval(Config.vsync?1:0);//启动垂直同步
        while(!this->is_exit){
            //  select render command queue  
            //  run command

            if(m_render_task_schedule != nullptr){
                m_render_task_schedule->tick();
            }
            
            const double current_time = CurrentTimeMircoDoubleFloat();
            double delta_time = current_time - last_time;
            last_time = current_time;
            elapsed_time += delta_time;
            if(elapsed_time >= 1000000.0f){
                m_render_fps = fps_counter;
                elapsed_time = 0.0f;
                fps_counter = 0;
            }else{
                fps_counter++;
            }

            glfwSwapBuffers(m_window);
        }//end while
            
        if(m_render_task_schedule != nullptr){
            m_render_task_schedule->clear();
        }

        glfwMakeContextCurrent(nullptr);
    }
}