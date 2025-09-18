#include <iostream>
#include "zidian.h"

class GameApp : public zidian::IApp{
public:
    zidian::SandBox *m_context;
    GameApp(zidian::SandBox *context) : m_context(context){
    }

    virtual void onInit(){
        zidian::Log::i("GameApp","onInit");
        // testJson();
       testSchedule();
    }

    virtual void onTick(float delta_time_micro){
        // testTime(delta_time_micro);
    }

    virtual void onDispose(){
        zidian::Log::i("GameApp","onDispose");
    }

    virtual ~GameApp(){
        zidian::Log::i("GameApp", "~GameApp destroy");
    }

    void testSchedule(){
        // m_context->getMainTaskSchedule()->schedule([this](void *){
        //     zidian::Log::green_log("logger", "run thread %lld main" , std::this_thread::get_id());

        //     m_context->getRenderTaskSchedule()->scheduleAtFixedRate([](void *){
        //         zidian::Log::blue_log("logger", "run render thread %lld" , std::this_thread::get_id());
        //     } , 1000L);
        // } , 3000L);

        m_context->getMainTaskSchedule()->scheduleAtFixedRate([this](void *){
            zidian::Log::green_log("logger", "logic fps:%d \t render fps: %d" , 
                m_context->m_logic_fps,
                m_context->m_render_fps);
        } , 1000L);
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

int main(int argc, char *argv[]){
    zidian::SandBox sandBox;
    zidian::AppParams param;

    param.name = "sand_box_game";
    param.view_width = 1280;
    param.view_height = 800;
    param.full_screen = false;
    param.vsync = true;
    param.render_backend = zidian::RenderBackend::Opengl;

    sandBox.init(param);
    
    GameApp *game = new GameApp(&sandBox);
    sandBox.setApp(game);
    int ret_code = sandBox.runLoop(argc, argv);
    delete game;
    return ret_code;
}