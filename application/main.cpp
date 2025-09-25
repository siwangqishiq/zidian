#include <iostream>
#include "zidian.h"
#include <string>
#include <cmath>

std::string audio_path = "";

class TestZidianRender : public zidian::IApp{
public:
    zidian::SandBox *m_context;
    zidian::ThreadPool *m_thread_pool = nullptr;

    bool is_play = false;

    float m_color_red = 0.0f;
    float m_delta_time = 0.0f;

    TestZidianRender(zidian::SandBox *context) : m_context(context){
    }

    virtual void onInit(){
        zidian::Log::i("GameApp","onInit");
        printFpsLog();
    }

    virtual void onTick(float delta_time_micro){
        zidian::Render2d::getInstance()->clearScreen();

        float view_width = zidian::Config.view_width;
        float view_height = zidian::Config.view_height;
        
        zidian::Render2d::getInstance()->drawPoint(0.0f, 0.0f, glm::vec4(1.0f, 0.0f , 0.0f , 1.0f));
        zidian::Render2d::getInstance()->drawPoint(view_width, 0.0f, glm::vec4(0.0f, 1.0f , 0.0f , 1.0f));
        zidian::Render2d::getInstance()->drawPoint(0.0f, view_height, glm::vec4(0.0f, 0.0f , 1.0f , 1.0f));
        zidian::Render2d::getInstance()->drawPoint(view_width, view_height, glm::vec4(1.0f, 1.0f , 0.0f , 1.0f));

        zidian::Render2d::getInstance()->drawPoint(view_width /2.0f, view_height/2.0f, glm::vec4(1.0f, 0.0f , 1.0f , 1.0f));
    }

    virtual void onDispose(){
        if(m_thread_pool != nullptr){
            m_thread_pool->shutdown();
            m_thread_pool = nullptr;
        }

        zidian::AudioManager::getInstance()->dispose();
        zidian::InputManager::getInstance()->clearCallback();
        zidian::Log::i("GameApp","onDispose");
    }

    virtual ~TestZidianRender(){
        zidian::Log::i("GameApp", "~GameApp destroy");
    }

    void printFpsLog(){
        m_context->getMainTaskSchedule()->scheduleAtFixedRate([this](void *){
            zidian::Log::blue_log("logger", "logic fps:%d \t render fps: %d" , 
                m_context->m_logic_fps,
                m_context->m_render_fps);
        } , 1000L);
    }
};

int main(int argc, char *argv[]){
    zidian::SandBox sandBox;
    zidian::AppParams param;

    if(argc >= 2){
        audio_path = argv[1];
    }

    param.name = "test_zidian_render";
    param.full_screen = false;
    param.vsync = true;
    param.window_boardless = false;
    param.render_backend = zidian::RenderBackend::Opengl;
    param.clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    sandBox.init(param);
    
    TestZidianRender *game = new TestZidianRender(&sandBox);
    sandBox.setApp(game);
    int ret_code = sandBox.runLoop(argc, argv);
    delete game;
    return ret_code;
}