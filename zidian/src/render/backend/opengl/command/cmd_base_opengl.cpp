#include "render/backend/opengl/command/cmd_base_opengl.h"

namespace zidian{
    CmdBaseOpenGL::~CmdBaseOpenGL(){
        if(m_vbo != 0){
            glDeleteBuffers(1, &m_vbo);
        }
        if(m_vao != 0){
            glDeleteVertexArrays(1, &m_vao);
        }
    }
}