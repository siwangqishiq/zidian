#pragma once

#include "glm/glm.hpp"
#include "config.h"
#include "render/paint.h"
#include "render/batch/ibatch_draw_points.h"
#include <thread>

namespace zidian{
    class IRender{
    public:
        virtual int init() = 0;
        virtual int initEvironment() = 0;

        virtual void setRenderThreadId(std::thread::id render_tid);

        virtual void dispose();

        virtual void onSizeChanged(int view_width, int view_height);

        virtual ~IRender(){};

        virtual std::shared_ptr<Cmd> createCommandInstance(int cmd_type);

        //----- batch ------
        virtual std::shared_ptr<IBatchDrawPoints> batchDrawPoints();

        //------draw methods----
        virtual void drawPoint(float &x, float &y , glm::vec4 &color, Paint &paint);

        virtual void setClearColor(glm::vec4 clear_color);

        virtual void clear();

        virtual bool checkIsRenderThread();
    private:
        std::thread::id m_render_tid;
    };
}

