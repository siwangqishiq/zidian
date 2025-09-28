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


    int m_img_width = 0;
    int m_img_height = 0;
    int m_img_channel = 0;

    glm::vec4 *m_pixels = nullptr;

    TestZidianRender(zidian::SandBox *context) : m_context(context){
    }

    virtual void onInit(){
        zidian::Log::i("GameApp","onInit");
        printFpsLog();
        // testReadImageFile();
        testReadAssetImageFile();
    }

    virtual void onTick(float delta_time_micro){
        // zidian::Render2d::getInstance()->clearScreen();
        auto start_time = zidian::CurrentTimeMillis();

        // test_case1();
        // testCase2();
        // testCase3RenderImage();
        testCase4();

        auto end_time = zidian::CurrentTimeMillis();
        // zidian::Log::e("log", "logic delta time = %lld", end_time - start_time);
    }

    void testCase4(){
        zidian::Render2d::getInstance()->clearScreen();
        zidian::Render2d::getInstance()->setClearColor(zidian::Colors::SKY_BLUE);

        zidian::Paint paint;
        for(int i = 0; i < 10;i++){
            for(int j = 0;j< 10; j++){
                zidian::Render2d::getInstance()->drawPoint(
                    static_cast<float>(j), 
                    static_cast<float>(i), 
                    m_pixels[m_img_width * i + j],paint);
            }
        }   
    }

    void testReadAssetImageFile(){
        uint8_t *data = 
            zidian::AssetManager::getInstance()->readAssetImageFile("images/p_128.jpg",
            m_img_width, m_img_height, m_img_channel);

        const int len = m_img_width * m_img_height;
        if(len > 0){
            m_pixels = new glm::vec4[len];
            for(int i = 0 ; i < len ;i++){
                if(m_img_channel == 3){
                    auto color = zidian::CreateColor(
                            data[i * m_img_channel + 0],
                            data[i * m_img_channel + 1],
                            data[i * m_img_channel + 2],
                            255
                        );
                    m_pixels[i] = color;
                }else if(m_img_channel == 4){
                    auto color = zidian::CreateColor(
                            data[i * m_img_channel + 0],
                            data[i * m_img_channel + 1],
                            data[i * m_img_channel + 2],
                            data[i * m_img_channel + 4]
                        );
                    m_pixels[i] = color;
                }
            }
        }


        if(data != nullptr){
            delete[] data;
        }

        // for(int i = 0;i < m_img_width * m_img_height ;i++){
        //     zidian::Log::green_log("pixels", "color<%f, %f, %f ,%f>", 
        //         m_pixels[i][0],m_pixels[i][1],m_pixels[i][2],m_pixels[i][3]);
        // }
        zidian::Log::green_log("pixels" ,"total pixels count :%d", m_img_width * m_img_height);
    }

    void testReadImageFile(){
        int width = 0;
        int height = 0;
        int channel = 0;

        uint8_t *data = 
            zidian::AssetManager::getInstance()->loadImageFileFromPath("d:/test.jpg",
            width, height, channel);

        if(data != nullptr){
            delete[] data;
        }
    }

    void testCase3RenderImage(){
        // zidian::Render2d::getInstance()->clearScreen();
        // zidian::Render2d::getInstance()->setClearColor(zidian::Colors::SKY_BLUE);

        zidian::Paint paint;
        for(int i = 0; i < m_img_height;i++){
            for(int j = 0;j< m_img_width; j++){
                zidian::Render2d::getInstance()->drawPoint(
                    static_cast<float>(j), 
                    static_cast<float>(i), 
                    m_pixels[m_img_width * i + j],paint);
            }
        }   
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
        if(m_pixels != nullptr){
            delete[] m_pixels;
            m_pixels = nullptr;
        }
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
    param.view_width = 1280;
    param.view_height = 720;
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