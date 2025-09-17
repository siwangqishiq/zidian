#include "render/render2d.h"
#include "utils/log.h"
#include "config.h"
#include "render/irender.h"

#include "render/backend/vulkan/vulkan_impl.h"
#include "render/backend/opengl/opengl_impl.h"

namespace zidian{
    const std::string Render2d::TAG = "render2d";
    std::unique_ptr<Render2d> Render2d::m_instance;

    std::unique_ptr<Render2d>& Render2d::getInstance(){
        if(m_instance == nullptr){
            m_instance = std::make_unique<Render2d>();
        }
        return m_instance;
    }

    Render2d::Render2d(){
        Log::i("render2d", "Render2d constructure");

        instanceRenderBackend();
    }

    void Render2d::instanceRenderBackend(){
        if(Config.render_backend == RenderBackend::Opengl){
            m_render = std::make_shared<OpenglRender>();
        }else if(Config.render_backend == RenderBackend::Vulkan){
            m_render = std::make_shared<VulkanRender>();
        }else{
        }

        if(m_render != nullptr){
            m_render->initEnv();
        }
    }

    Render2d::~Render2d(){
        Log::i("render2d", "Render2d destroy");
    }
}
