#include "render/render2d.h"
#include "utils/log.h"
#include "config.h"
#include "render/irender.h"

#include "render/backend/vulkan/vulkan_impl.h"
#include "render/backend/opengl/opengl_impl.h"

#include "render/command/cmd.h"
#include "render/command/cmd_draw_point.h"

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
        m_command_queue = std::make_unique<CommandQueue>();
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

    void Render2d::executeRenderCommands(){
        std::vector<CmdQueueType>& cmds = m_command_queue->getRenderBuffer();
        for(CmdQueueType& cmd : cmds){
            cmd->execute();
        }
        // cmds.clear();
        // Log::blue_log("render_thread" , "executeRenderCommands end. %d", std::this_thread::get_id());
    }

    std::vector<CmdQueueType>& Render2d::getCommandBuffer(){
        return m_command_queue->getWriteBuffer();
    }

    void Render2d::submitCommandBuffer(){
        m_command_queue->submitWriteBuffer();
    }

    void Render2d::addCmd(CmdQueueType cmd){
        std::vector<CmdQueueType>& buffer = m_command_queue->getWriteBuffer();
        buffer.emplace_back(cmd);
    }

    void Render2d::drawPoint(float x, float y, glm::vec4 color){
        auto cmd = std::make_shared<CmdDrawPoint>(m_render.get());
        cmd->putParams(x, y, color);
        addCmd(cmd);
    }
}
