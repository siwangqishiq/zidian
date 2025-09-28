#pragma once

#include <string>
#include <memory>
#include "render/backend_enum.h"
#include <memory>
#include "config.h"
#include "glm/glm.hpp"
#include "render/command_queue.h"
#include <mutex>
#include "render/paint.h"


namespace zidian{
    class IRender;
    class CommandPool;
    class CommandQueue;

    class Render2d{
    public:
        static const std::string TAG;
        
        static std::unique_ptr<Render2d>& getInstance();

        Render2d(const Render2d&) = delete;
        Render2d& operator=(const Render2d&) = delete;    

        Render2d();

        ~Render2d();

        int getViewWidth();

        int getViewHeight();

        void onSizeChanged(int view_width, int view_height);

        void instanceRenderBackend();

        void executeRenderCommands();

        void init();

        void onRenderStart();

        void dispose();

        std::shared_ptr<IRender> getRender(){
            return m_render;
        }

        void onStartRenderFrame();

        std::vector<CmdQueueType>& getCommandBuffer();
        
        void onAfterSubmitCommandBuffer();

        void submitCommandBuffer();
        
        void addCmd(CmdQueueType cmd);

        //清理屏幕
        void clearScreen();

        void setClearColor(ColorType color);

        void drawPoint(float x, float y, glm::vec4 color, Paint paint);

        std::unique_ptr<CommandQueue>& getCommandQueue();

        glm::mat3 m_screen_ndc_matrix;
    private:
        static std::mutex m_mutex;
        static std::unique_ptr<Render2d> m_instance;

        std::shared_ptr<IRender> m_render = nullptr;

        std::unique_ptr<CommandQueue> m_command_queue;
    };
}
