#pragma once

#include "render/command/cmd.h"
#include "render/backend/opengl/glheader.h"


namespace zidian{
    class CmdBaseOpenGL : public Cmd{
    public:
        CmdBaseOpenGL(int cmd_type):Cmd(cmd_type){}

        ~CmdBaseOpenGL();
    protected:
        GLuint m_vao = 0;
        GLuint m_vbo = 0;
    };//end class
}