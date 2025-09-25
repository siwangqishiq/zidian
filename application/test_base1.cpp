#include <iostream>
#include "zidian.h"
#include <string>
#include <cmath>

std::string audio_path = "";

class GameApp : public zidian::IApp{
public:
    zidian::SandBox *m_context;
    zidian::ThreadPool *m_thread_pool = nullptr;

    bool is_play = false;

    float m_color_red = 0.0f;
    float m_delta_time = 0.0f;

    GameApp(zidian::SandBox *context) : m_context(context){
    }

    virtual void onInit(){
        zidian::Log::i("GameApp","onInit");
        printFpsLog();
        
        // testJson();
        // testSchedule();
        // testThreadPool();
        // testAssetManager();
        // testAudioPlay();
        // testInput();
        // testInputAndAudio();
    }

    virtual void onTick(float delta_time_micro){
        // testTime(delta_time_micro);
        // zidian::Log::i("state","key space state :%d", 
        //     zidian::InputManager::getInstance()->getKeyState(zidian::CODE_KEY_SPACE));
        // testPlayAudioTick();

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

    virtual ~GameApp(){
        zidian::Log::i("GameApp", "~GameApp destroy");
    }

    void testChangeClearColor(float delta_time_micro){
        zidian::Render2d::getInstance()->clearScreen();

        zidian::Render2d::getInstance()->drawPoint(1.0f, 2.0f, glm::vec4(1.0f, 0.0f , 0.0f , 1.0f));
        zidian::Render2d::getInstance()->drawPoint(3.0f, 4.0f, glm::vec4(0.0f, 0.0f , 0.0f , 1.0f));
        m_delta_time += delta_time_micro/1000000.0f;
        m_color_red = 0.5f * std::sin(m_delta_time) + 0.5f;
        // zidian::Log::i("tick","key space color :%f   time : %f", m_color_red , m_delta_time);
        zidian::Render2d::getInstance()->setClearColor(glm::vec4(m_color_red, 0.0f, 1.0f - m_color_red ,1.0f));
    }

    void testPlayAudioTick(){
        bool play = false;
        if(zidian::InputManager::getInstance()->getKeyState(zidian::CODE_KEY_SPACE) ==
            zidian::KEY_PRESS){
            play = false;
        }else{
            play = true;
        }

        if(play != is_play){
            if(play){
                zidian::AudioManager::getInstance()->playAudio("bgm");
            }else{
                zidian::AudioManager::getInstance()->stopAudio("bgm");
            }
        }
        is_play = play;
    }

    void testInputAndAudio(){
        if(audio_path.empty()){
            auto music_audio = zidian::AudioManager::getInstance()->loadAudio("audio/sandong.mp3","bgm",true);
        }else{
            zidian::Log::w("audio" , "play audio file: %s", audio_path.c_str());
            auto music_audio = zidian::AudioManager::getInstance()->loadAudio(audio_path,"bgm",true, false);
        }
        zidian::AudioManager::getInstance()->playAudio("bgm");
        this->is_play = true;
    }

    void testInput(){
        zidian::InputManager::getInstance()->addEventListener("input", 
        [](zidian::InputEvent e){
            zidian::Log::i("input_event", "input %d, %f %f", e.action, e.x, e.y);
            return true;
        });
    }

    void testAudioPlay(){
        std::shared_ptr<zidian::AudioEntity> audio_entity;
        if(audio_path.empty()){
            audio_entity = zidian::AudioManager::getInstance()->loadAudio("audio/sandong.mp3","bgm",true);
        }else{
            zidian::Log::w("audio" , "play audio file: %s", audio_path.c_str());
            audio_entity = zidian::AudioManager::getInstance()->loadAudio(audio_path,"bgm",true, false);
        }
        zidian::AudioManager::getInstance()->playAudio("bgm");

        zidian::AudioManager::getInstance()->setAudioPlayProgressCallback(audio_entity, 
            [](unsigned long cur, unsigned long total, double p){
                zidian::Log::i("audio","progress : %u / %u", cur, total);
            }
        );
    }

    void testAssetManager(){
        // zidian::Log::i("GameApp", "asset dir path:%s", 
        //     zidian::AssetManager::getInstance()->assetRootDir().c_str());

        zidian::Log::i("GameApp", "read file size:%s", 
            zidian::AssetManager::getInstance()->readAssetFileAsString("test.txt").c_str());
        long file_length = 0;
        auto data = zidian::AssetManager::getInstance()->readAssetFileAsBinary("test.txt", file_length);
        zidian::Log::i("GameApp", "read bin file size:%ld", file_length);
    }

    void testThreadPool(){
        m_thread_pool = new zidian::ThreadPool(2, zidian::ScheduleMode::FIFO);
        m_thread_pool->enqueue([](){
            while(true){
                std::cout << "A";
            }
            
        });
        m_thread_pool->enqueue([](){
            while(true){
                std::cout << "B";
            }
        });
    }

    void printFpsLog(){
        m_context->getMainTaskSchedule()->scheduleAtFixedRate([this](void *){
            zidian::Log::blue_log("logger", "logic fps:%d \t render fps: %d" , 
                m_context->m_logic_fps,
                m_context->m_render_fps);
        } , 1000L);
    }

    void testSchedule(){
        printFpsLog();
    }

    void testJson(){
        zidian::JsonObject obj;
        obj.putInt("age",36);
        obj.putString("name", L"紫电青霜");
        auto str = obj.toJsonString();
        zidian::Log::i("GameApp", "init json:%s", zidian::ToByteString(str).c_str());
        // zidian::WriteStringToFile("json.txt", str);
    }

    void testTime(float delta_time_micro){
        zidian::Log::i("GameApp","onTick");
        auto time_mirco = zidian::CurrentTimeMicro();
        auto time_mil = zidian::CurrentTimeMillis();
        auto time_flt = zidian::CurrentTimeMillisDoubleFloat();
        auto time_seconds = zidian::CurrentTimeSeconds();
        double time_micro_flt = zidian::CurrentTimeMircoDoubleFloat();
        zidian::Log::i("GameApp","time :%lld  %lld  %f %f %lld  deltatime: %f",
            time_mirco, time_mil , 
            time_flt, time_micro_flt, 
            time_seconds , delta_time_micro);
        
        zidian::Log::i("GameApp", "dealta time : %f mills", delta_time_micro / 1000.0f);
    }
};

int main_test1(int argc, char *argv[]){
    zidian::SandBox sandBox;
    zidian::AppParams param;

    if(argc >= 2){
        audio_path = argv[1];
    }

    param.name = "sand_box_game";
    param.view_width = 800;
    param.view_height = 600;
    param.full_screen = false;
    param.vsync = true;
    param.window_boardless = false;
    param.render_backend = zidian::RenderBackend::Opengl;
    param.clear_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    sandBox.init(param);
    
    GameApp *game = new GameApp(&sandBox);
    sandBox.setApp(game);
    int ret_code = sandBox.runLoop(argc, argv);
    delete game;
    return ret_code;
}