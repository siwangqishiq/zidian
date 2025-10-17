#include "zidian.h"
#include <iostream>

#ifndef __ANDROID__
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif

#include "render/irender.h"

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

        //vulkan实现
        if(Config.render_backend == RenderBackend::Vulkan){
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }

        Render2d::getInstance()->init();

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
            SandBox* sandbox = static_cast<SandBox *>(glfwGetWindowUserPointer(windows_));
            sandbox->getRenderTaskSchedule()->schedule([w,h](void *){
                zidian::Render2d::getInstance()->onSizeChanged(w, h);
            },0);
            
            if(sandbox->m_app != nullptr){
                sandbox->m_app->onResize(w, h);
            }
        });

        glfwSetWindowPos(m_window, 32, 64);
        Global.windows = m_window;
        
        glfwSetWindowUserPointer(m_window, static_cast<void *>(this));
        m_render_thread = std::thread([this](){
           renderThreadFunc();
        });

        mainThreadFunc();

        this->is_exit = true;
        m_render_thread.join();
        glfwDestroyWindow(m_window);
        glfwTerminate();
        
        Log::w(TAG, "sand box ended.");

        return 0;
    }

    //主线程
    void SandBox::mainThreadFunc(){
        m_main_tid = std::this_thread::get_id();
        Render2d::getInstance()->onRenderStart();
        
        m_main_task_schedule = std::make_unique<TaskSchedule>();

        if(m_app != nullptr){
            m_app->onInit();
        }

        m_last_time_micro = CurrentTimeMircoDoubleFloat();
        int fps_counter = 0;
        double elapsed_time = 0.0f;
        
        InputManager::getInstance()->setWindowInstance(m_window);

        while(!glfwWindowShouldClose(m_window)) {
            Render2d::getInstance()->getCommandBuffer();

            Render2d::getInstance()->onStartRenderFrame();
            
            //handle input
            glfwPollEvents();
            if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                this->is_exit = true;
                Render2d::getInstance()->submitCommandBuffer();
                break;
            }

            if(m_app != nullptr){
                m_app->onTick(m_delta_time_micro);
            }

            if(m_main_task_schedule != nullptr){
                m_main_task_schedule->tick();
            }

            Render2d::getInstance()->submitCommandBuffer();
            updateTimeStamp(elapsed_time , fps_counter);
        }//end while

        if(m_main_task_schedule != nullptr){
            m_main_task_schedule->clear();
        }

        if(m_app != nullptr){
            m_app->onDispose();
        }

        Render2d::getInstance()->dispose();
    }

    //渲染线程
    void SandBox::renderThreadFunc(){
        Log::w(TAG,"start render thread: %ld", std::this_thread::get_id());
        m_render_tid = std::this_thread::get_id();

        glfwMakeContextCurrent(m_window);
  
        m_render_task_schedule = std::make_unique<TaskSchedule>();

        auto render = Render2d::getInstance()->getRender();
        render->setRenderThreadId(m_render_tid);
        render->initEvironment();

        // error debug in error thread  
        // render->setClearColor(Config.clear_color);

        double elapsed_time = 0.0f;
        double last_time = CurrentTimeMircoDoubleFloat();
        int fps_counter = 0;

        glfwSwapInterval(Config.vsync?1:0);//启动垂直同步
        while(!this->is_exit){
            auto start_time = CurrentTimeMillis();
            
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

            auto end_time = CurrentTimeMillis();
            // Log::e("render_thread", "render one frame costtime :%lld", end_time - start_time);
            glfwSwapBuffers(m_window);
        }//end while
            
        if(m_render_task_schedule != nullptr){
            m_render_task_schedule->clear();
        }

        render->dispose();

        glfwMakeContextCurrent(nullptr);
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
}