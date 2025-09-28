#include "render/render2d.h"
#include "utils/log.h"
#include "config.h"
#include "render/irender.h"

#include "render/backend/vulkan/vulkan_impl.h"
#include "render/backend/opengl/opengl_impl.h"

#include "render/command/types.h"
#include "render/command/command_pool.h"
#include "render/command/cmd.h"

namespace zidian{
    const std::string Render2d::TAG = "render2d";
    std::unique_ptr<Render2d> Render2d::m_instance;
    std::mutex Render2d::m_mutex;

    std::unique_ptr<Render2d>& Render2d::getInstance(){
        if(m_instance == nullptr){
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_instance == nullptr){
                m_instance = std::make_unique<Render2d>();
            }
        }
        return m_instance;
    }

    Render2d::Render2d() {
        Log::i("render2d", "Render2d constructure");
        instanceRenderBackend();

        m_command_queue = std::make_unique<CommandQueue>(m_render.get());
    }

    int Render2d::getViewWidth(){
        return Config.view_width;
    }

    int Render2d::getViewHeight(){
        return Config.view_height;
    }

    // 创建不同的图形API后端
    // 目前先仅支持 Opengl/opengles 4.1
    // 后续添加 Vulkan DirectX_12 等的支持
    void Render2d::instanceRenderBackend(){
        if(Config.render_backend == RenderBackend::Opengl){
            m_render = std::make_shared<OpenglRender>();
        }else if(Config.render_backend == RenderBackend::Vulkan){
            m_render = std::make_shared<VulkanRender>();
        }else{
            //other graph backend
        }
    }

    void Render2d::init(){
        if(m_render != nullptr){
            m_render->init();
        }
    }

    void Render2d::onSizeChanged(int view_width, int view_height){
        Log::i("render2d", "window size change to %d, %d", view_width , view_height);
        Config.view_width = view_width;
        Config.view_height = view_height;

        if(m_render != nullptr){
            m_render->onSizeChanged(view_width, view_height);
        }
    }

    void Render2d::onRenderStart(){
        
    }

    void Render2d::dispose(){
        
    }

    Render2d::~Render2d(){
        Log::i("render2d", "Render2d destroy");
    }

    std::unique_ptr<CommandQueue>& Render2d::getCommandQueue(){
        return m_command_queue;
    }

    void Render2d::executeRenderCommands(){
        std::vector<CmdQueueType>& cmds = m_command_queue->getRenderBuffer();
        for(CmdQueueType& cmd : cmds){
            cmd->execute();
        }
        // Log::blue_log("render_thread" , "executeRenderCommands end. %d", std::this_thread::get_id());
    }

    std::vector<CmdQueueType>& Render2d::getCommandBuffer(){
        return m_command_queue->getWriteBuffer();
    }

    void Render2d::submitCommandBuffer(){
        m_command_queue->submitWriteBuffer();
        onAfterSubmitCommandBuffer();
    }

    void Render2d::onStartRenderFrame(){
        std::shared_ptr<CommandPool>& cmd_pool = m_command_queue->getCurrentCommandPool();
        // cmd_pool->debugPrintInfo();
        cmd_pool->resetPool();
        // Log::blue_log("command_pool", "after reset");
        // cmd_pool->debugPrintInfo();
    }

    void Render2d::onAfterSubmitCommandBuffer(){
    }

    void Render2d::addCmd(CmdQueueType cmd){
        m_command_queue->getWriteBuffer().emplace_back(cmd);
    }

    void Render2d::clearScreen(){
        // auto cmd = m_command_queue->getCurrentCommandPool()->getCommandByType(CMD_TYPE_CLEAR);
        // addCmd(cmd);
        m_render->clear();
    }

    void Render2d::setClearColor(ColorType color){
        m_render->setClearColor(color);
    }

    void Render2d::drawPoint(float x, float y, glm::vec4 color, Paint paint){
        m_render->drawPoint(x, y, color, paint);
    }
}
