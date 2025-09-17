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
        Log::w(TAG,"start main thread: %ld", std::this_thread::get_id());
        
        glfwInit();

        m_window = glfwCreateWindow(800, 600, Config.name.c_str(), nullptr, nullptr);
        
        m_render_thread = std::thread([this](){
           renderThreadFunc();
        });

        mainThreadFunc();

        this->is_exit = true;
        // std::this_thread::sleep_for(std::chrono::milliseconds(200));
        m_render_thread.join();
        glfwDestroyWindow(m_window);
        glfwTerminate();
        Log::w(TAG,"run loop end prepare");

        // std::this_thread::sleep_for(std::chrono::seconds(5));
        Log::w(TAG,"run loop end");
        return 0;
    }

    void SandBox::mainThreadFunc(){
        if(m_app != nullptr){
            m_app->onInit();
        }

        m_last_time_mills = CurrentTimeMillisDoubleFloat();
        while(!glfwWindowShouldClose(m_window)) {
            updateTimeStamp();
            glfwPollEvents();

            if(m_app != nullptr){
                m_app->onTick(m_delta_time_mills);
            }

            if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                this->is_exit = true;
                break;
            }

            if(Config.vsync){
                glfwSwapInterval(1);//锁定固定帧率
            }
        }//end while

        if(m_app != nullptr){
            m_app->onDispose();
        }
    }

    void SandBox::updateTimeStamp(){
        double cur_time = CurrentTimeMillisDoubleFloat();
        m_delta_time_mills = cur_time - m_last_time_mills;
        m_last_time_mills = cur_time;
    }

    void SandBox::renderThreadFunc(){
        Log::w(TAG,"start render thread: %ld", std::this_thread::get_id());

        glfwMakeContextCurrent(m_window);

        Render2d::getInstance();

        while(!this->is_exit){
            //  select render command queue  
            //  run command
            glfwSwapBuffers(m_window);
        }//end while

        glfwMakeContextCurrent(nullptr);
    }
}