#pragma once

#include <string>
#include <memory>
#include "render/backend_enum.h"
#include <memory>
#include "config.h"
#include "glm/glm.hpp"
#include "render/command_queue.h"
#include <mutex>

namespace zidian{
    class IRender;

    class Render2d{
    public:
        static const std::string TAG;
        
        static std::unique_ptr<Render2d>& getInstance();

        Render2d();

        ~Render2d();

        void instanceRenderBackend();

        void executeRenderCommands();

        std::vector<CmdQueueType>& getCommandBuffer();

        void submitCommandBuffer();

        void addCmd(CmdQueueType cmd);

        void drawPoint(float x, float y, glm::vec4 color);
    private:
        static std::mutex m_mutex;
        static std::unique_ptr<Render2d> m_instance;
        std::shared_ptr<IRender> m_render = nullptr;
        std::unique_ptr<CommandQueue> m_command_queue;
    };
}
