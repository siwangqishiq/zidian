#pragma once

#include "render/irender.h"
#include "render/command/cmd.h"

namespace zidian{
    class OpenglRender : public IRender{
    public:
        static const std::string UNIFORM_NAME_SCRTONDC_MAT;
        static const std::string UNIFORM_NAME_POINTSIZE;

        virtual void onSizeChanged(int view_width, int view_height) override;

        virtual int init() override;

        virtual int initEvironment() override;

        virtual void dispose() override;

        virtual std::shared_ptr<Cmd> createCommandInstance(int cmd_type) override;
        
        virtual void clear() override;
        virtual void setClearColor(glm::vec4 clear_color) override;
        virtual void drawPoint(float &x, float &y , glm::vec4 &color, Paint &paint) override;
    private:
        void initOpenglEnv();
    };
}


