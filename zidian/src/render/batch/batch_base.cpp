#include "render/batch/batch_base.h"

namespace zidian{
    Batch::Batch(IRender *render){
        this->m_render = render;
    }

    Batch::~Batch(){

    }
    
    void Batch::begin(){

    }
    
    void Batch::end(){

    }
}