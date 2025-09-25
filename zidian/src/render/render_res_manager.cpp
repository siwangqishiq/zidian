#include "render/render_res_manager.h"


namespace zidian{
    RenderResManager::RenderResManager(){
        m_next_handle = 0;
    }

    RenderResManager::~RenderResManager(){
    }
    
    RenderResHandle RenderResManager::genResHandle(){
        RenderResHandle handle = m_next_handle++;
        return handle;
    }
}

