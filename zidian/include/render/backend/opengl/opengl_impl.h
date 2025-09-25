#pragma once

#include "render/irender.h"
#include "render/backend/opengl/glheader.h"
#include "render/backend/opengl/shader.h"
#include "render/backend/opengl/shader_constants.h"

namespace zidian{
    class OpenglRender : public IRender{
    public:
        virtual int init() override;

        virtual int initEvironment() override;

        virtual void dispose() override;

        virtual void clear() override;
        virtual void setClearColor(glm::vec4 clear_color) override;
        virtual void drawPoint(float &x, float &y , glm::vec4 &color) override;
    private:
        GLuint m_vao;

        GLuint m_draw_point_vao;
        GLuint m_draw_point_buffer;

        void initOpenglEnv();

        void prepareDrawPoint();
    };
}


