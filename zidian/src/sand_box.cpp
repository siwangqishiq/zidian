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

        if(Config.window_boardless){
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        }

        m_window = glfwCreateWindow(Config.view_width, Config.view_height, 
            Config.name.c_str(), m_monitor, nullptr);

        glfwSetKeyCallback(m_window , [](GLFWwindow* windows_,int key,
                int scancode,int action,int mods){
            zidian::InputEvent event;
            event.code = key;
            if(action == GLFW_PRESS){
                event.action = zidian::EVENT_ACTION_KEYBOARD_PRESS;
            }else if(action == GLFW_RELEASE){
                event.action = zidian::EVENT_ACTION_KEYBOARD_RELEASE;
            }else if(action == GLFW_REPEAT){
                event.action = zidian::EVENT_ACTION_KEYBOARD_REPEAT;
            }

            zidian::InputManager::getInstance()->onEvent(event);
        });

        glfwSetCursorPosCallback(m_window , [](GLFWwindow* window, double xpos, 
                double ypos){
            if(zidian::InputManager::getInstance()->mouse_left_pressed){
                zidian::InputEvent event;
                event.action = zidian::EVENT_ACTION_MOVE;
                event.x = xpos;
                event.y = ypos;
                zidian::InputManager::getInstance()->onEvent(event);
            }
            
            if(zidian::InputManager::getInstance()->mouse_middle_pressed){
                zidian::InputEvent event;
                event.action = zidian::EVENT_ACTION_MOUSE_MIDDLE_MOVE;
                event.x = xpos;
                event.y = ypos;

                zidian::InputManager::getInstance()->onEvent(event);
            }
            
            if(zidian::InputManager::getInstance()->mouse_right_pressed){
                zidian::InputEvent event;
                event.action = zidian::EVENT_ACTION_MOUSE_RIGHT_MOVE;
                event.x = xpos;
                event.y = ypos;
                
                zidian::InputManager::getInstance()->onEvent(event);
            }
        });

        glfwSetMouseButtonCallback(m_window , [](GLFWwindow* window, int button, int action, int mods){
            zidian::InputEvent event;
            
            if(button == GLFW_MOUSE_BUTTON_LEFT){
                if(action == GLFW_PRESS){
                    zidian::InputManager::getInstance()->mouse_left_pressed = true;
                    event.action = zidian::EVENT_ACTION_BEGIN; 
                }else if(action == GLFW_RELEASE){
                    zidian::InputManager::getInstance()->mouse_left_pressed = false;
                    event.action = zidian::EVENT_ACTION_END;
                }
            }else if(button == GLFW_MOUSE_BUTTON_MIDDLE){
                if(action == GLFW_PRESS){
                    zidian::InputManager::getInstance()->mouse_middle_pressed = true;
                    event.action = zidian::EVENT_ACTION_MOUSE_MIDDLE_BEGIN; 
                }else if(action == GLFW_RELEASE){
                    zidian::InputManager::getInstance()->mouse_middle_pressed = false;
                    event.action = zidian::EVENT_ACTION_MOUSE_MIDDLE_END; 
                }
            }else if(button == GLFW_MOUSE_BUTTON_RIGHT){
                if(action == GLFW_PRESS){
                    zidian::InputManager::getInstance()->mouse_right_pressed = true;
                    event.action = zidian::EVENT_ACTION_MOUSE_RIGHT_BEGIN; 
                }else if(action == GLFW_RELEASE){
                    zidian::InputManager::getInstance()->mouse_right_pressed = false;
                    event.action = zidian::EVENT_ACTION_MOUSE_RIGHT_END; 
                }
            }
            double x = 0;
            double y = 0;
            glfwGetCursorPos(window, &x, &y);
            event.x = x;
            event.y = y;

            zidian::InputManager::getInstance()->onEvent(event);
        });

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* windows_,int w,int h){
        });

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
        
        InputManager::getInstance()->setWindowInstance(m_window);

        while(!glfwWindowShouldClose(m_window)) {
            Render2d::getInstance()->getCommandBuffer().clear();
            glfwPollEvents();

            if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                this->is_exit = true;
                Render2d::getInstance()->submitCommandBuffer();
                break;
            }

            updateTimeStamp(elapsed_time , fps_counter);

            if(m_app != nullptr){
                m_app->onTick(m_delta_time_micro);
            }
            
            if(m_main_task_schedule != nullptr){
                m_main_task_schedule->tick();
            }

            Render2d::getInstance()->submitCommandBuffer();
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
            Render2d::getInstance()->executeRenderCommands();

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