#include "render/backend/opengl/command/cmd_draw_point.h"
#include "utils/log.h"
#include "render/irender.h"
#include "render/backend/opengl/shader.h"
#include "render/backend/opengl/shader_constants.h"

namespace zidian{
    void CmdDrawPoint::putParams(float x, float y, glm::vec4 color , Paint paint){
        m_x = x;
        m_y = y;
        m_color = color;
        m_paint = paint;
    }

    CmdDrawPoint::~CmdDrawPoint(){
        glDeleteBuffers(1, &m_draw_point_buffer);
        glDeleteVertexArrays(1, &m_draw_point_vao);
    }

    void CmdDrawPoint::prepareDrawPoint(){
        auto draw_point_shader = ShaderManager::getInstance()->loadAssetShader(ShaderMetas::DRAW_POINT);
        Log::i("opengl_render", "draw_point_shader programID = %u", draw_point_shader.m_programId);

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

        Log::e("test", "m_draw_point_buffer = %d", m_draw_point_buffer);
    }

    void CmdDrawPoint::execute() {
        if(m_draw_point_buffer == 0){
            prepareDrawPoint();
        }

        auto draw_point_shader = ShaderManager::getInstance()->getShaderByName(ShaderMetas::DRAW_POINT.name);
        if(draw_point_shader.m_programId == 0){
            return;
        }

        draw_point_shader.useShader();
        draw_point_shader.setUniformMat3(UNIFORM_NAME_SCRTONDC_MAT, 
            Render2d::getInstance()->m_screen_ndc_matrix);
        draw_point_shader.setUniformFloat(UNIFORM_NAME_POINTSIZE, m_paint.point_size);

        //update data
        float pointPos[2 + 4] = { m_x, m_y , m_color[0], m_color[1], m_color[2], m_color[3]};
        glBindBuffer(GL_ARRAY_BUFFER, m_draw_point_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointPos), pointPos);

        glBindVertexArray(m_draw_point_vao);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
    }
}