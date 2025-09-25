#include "render/backend/opengl/opengl_impl.h"
#include "utils/log.h"

#ifndef __ANDROID__
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif

#include "config.h"

namespace zidian{
    int OpenglRender::init() {
        Log::w("render", "init OpengGL render");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        return 0;
    }

    int OpenglRender::initEvironment(){
#ifndef ANDROID
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Log::e("render", "init OpengGL environment error");
            return -1;
        }
#endif

        initOpenglEnv();
        return 0;
    }

    void OpenglRender::initOpenglEnv(){
        GLuint vaos[1];
        glGenVertexArrays(1,vaos);
        m_vao = vaos[0];

        glEnable(GL_PROGRAM_POINT_SIZE);
        glViewport(0, 0, Config.view_width, Config.view_height);

        prepareDrawPoint();
    }

    void OpenglRender::prepareDrawPoint(){
        GLuint vaos[1];
        glGenVertexArrays(1,vaos);
        m_draw_point_vao = vaos[0];

        GLuint vbo[1];
        glGenBuffers(1, vbo);
        m_draw_point_buffer = vbo[0];
        
        glBindVertexArray(m_draw_point_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_draw_point_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (2 + 4), nullptr, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (2 + 4) * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (2 + 4) * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        auto draw_point_shader = ShaderManager::getInstance()->loadAssetShader(ShaderMetas::DRAW_POINT);
        Log::i("opengl_render", "draw_point_shader programID = %u", draw_point_shader.m_programId);
    }

    void OpenglRender::drawPoint(float &x, float &y , glm::vec4 &color){
        auto draw_point_shader = ShaderManager::getInstance()->getShaderByName(ShaderMetas::DRAW_POINT.name);
        if(draw_point_shader.m_programId == 0){
            return;
        }

        // Log::i("opengl_render", "draw_point_shader programID = %u", draw_point_shader.m_programId);
        draw_point_shader.useShader();

        //update data
        float pointPos[2 + 4] = { x, y , color[0], color[1], color[2], color[3]};
        glBindBuffer(GL_ARRAY_BUFFER, m_draw_point_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointPos), pointPos);

        glBindVertexArray(m_draw_point_vao);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    }

    void OpenglRender::dispose() {
        glDeleteBuffers(1, &m_draw_point_buffer);
        glDeleteVertexArrays(1, &m_draw_point_vao);

        ShaderManager::getInstance()->clear();
    }

    void OpenglRender::setClearColor(glm::vec4 clear_color){
        glClearColor(clear_color[0], clear_color[1] ,clear_color[2] ,clear_color[3]);
    }

    void OpenglRender::clear(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}


