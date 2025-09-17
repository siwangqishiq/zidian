#include <iostream>
#include "zidian.h"

class GameApp : public zidian::IApp{
public:
    virtual void onInit(){
        zidian::Log::i("GameApp","onInit");
    }

    virtual void onTick(){
        // zidian::Log::i("GameApp","onTick");
        auto time_mirco = zidian::CurrentTimeMicro();
        auto time_mil = zidian::CurrentTimeMillis();
        auto time_flt = zidian::CurrentTimeMillisFloat();
        auto time_seconds = zidian::CurrentTimeSeconds();
        zidian::Log::i("GameApp","time :%lld  %lld  %f %lld",time_mirco, time_mil , 
            time_flt, time_seconds);
    }

    virtual void onDispose(){
        zidian::Log::i("GameApp","onDispose");
    }

    virtual ~GameApp(){
    }
};

int main(int argc, char *argv[]){
    zidian::SandBox sandBox;
    zidian::AppParams param;

    param.name = "sand_box_game";
    param.render_backend = zidian::RenderBackend::Opengl;

    sandBox.init(param);
    
    GameApp *game = new GameApp();
    sandBox.setApp(game);
    int ret_code = sandBox.runLoop(argc, argv);
    delete game;
    return ret_code;
}