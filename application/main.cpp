#include <iostream>
#include "zidian.h"
#include <string>
#include <cmath>

extern std::string audio_path;

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
        // zidian::Render2d::getInstance()->clearScreen();
        auto start_time = zidian::CurrentTimeMillis();

        // test_case1();
        testCase2();

        auto end_time = zidian::CurrentTimeMillis();
        // zidian::Log::e("log", "logic delta time = %lld", end_time - start_time);
    }

    void testCase2(){
        zidian::Render2d::getInstance()->setClearColor(zidian::Colors::BLUE);

        zidian::Render2d::getInstance()->clearScreen();

        float view_width = zidian::Config.view_width;
        float view_height = zidian::Config.view_height;

        zidian::Paint paint;
        for(int i = 0; i < 10;i++){
            for(int j = 0;j< 10; j++){
                zidian::Render2d::getInstance()->drawPoint(
                    static_cast<float>(j), 
                    static_cast<float>(i), 
                    glm::vec4(0.0f, 1.0f , 0.0f , 1.0f),paint);
            }
        }
    }

    void test_case1(){
        float view_width = zidian::Config.view_width;
        float view_height = zidian::Config.view_height;

        zidian::Paint paint;

        zidian::Render2d::getInstance()->drawPoint(0.0f, 0.0f, 
            glm::vec4(1.0f, 0.0f , 0.0f , 1.0f),paint);
        zidian::Render2d::getInstance()->drawPoint(view_width, 
            0.0f, glm::vec4(0.0f, 1.0f , 0.0f , 1.0f) , paint);
        zidian::Render2d::getInstance()->drawPoint(0.0f, view_height, 
            glm::vec4(0.0f, 0.0f , 1.0f , 1.0f), paint);

        zidian::Render2d::getInstance()->drawPoint(view_width, view_height, 
            glm::vec4(1.0f, 1.0f , 0.0f , 1.0f), paint);

        paint.point_size = 100.0f;
        zidian::Render2d::getInstance()->drawPoint(view_width /2.0f, view_height/2.0f, 
            glm::vec4(1.0f, 0.0f , 1.0f , 1.0f), paint);
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
    param.view_width = 1080;
    param.view_height = 800;
    param.window_boardless = false;
    param.render_backend = zidian::RenderBackend::Opengl;
    param.clear_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

    sandBox.init(param);
    
    TestZidianRender *game = new TestZidianRender(&sandBox);
    sandBox.setApp(game);
    int ret_code = sandBox.runLoop(argc, argv);
    delete game;
    return ret_code;
}