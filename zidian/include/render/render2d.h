#pragma once

#include <string>
#include <memory>
#include "render/backend_enum.h"
#include <memory>

namespace zidian{
    class IRender;
    class Render2d{
    public:
        static const std::string TAG;
        
        static std::unique_ptr<Render2d>& getInstance();

        Render2d();
        ~Render2d();

        void instanceRenderBackend();
    private:
        static std::unique_ptr<Render2d> m_instance;

        std::shared_ptr<IRender> m_render = nullptr;
    };
}
