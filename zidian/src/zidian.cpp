#include "zidian.h"
#include <iostream>

namespace zidian{

    void SandBox::init(AppParams params){
        this->m_params = params;
    }

    int SandBox::runLoop(int argc, char **argv){
        for(int i = 0 ; i < 10 ; i++){
            std::cout << m_params.name << " SandBox run loop... \n";
        }
        
        return 0;
    }

}