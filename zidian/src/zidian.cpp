#include "zidian.h"
#include <iostream>

namespace zidian{
    const std::string TAG = "zidian";

    void SandBox::init(AppParams params){
        this->m_params = params;
    }

    int SandBox::runLoop(int argc, char **argv){
        Log::w(TAG,"run loop start");
        for(int i = 0 ; i < 10 ; i++){
            Log::i(TAG,"run loop %d", i);
        }//end for i
        Log::e(TAG,"run loop end");
        return 0;
    }
}