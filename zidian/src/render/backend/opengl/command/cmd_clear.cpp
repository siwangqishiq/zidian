
#include "render/backend/opengl/glheader.h"
#include "render/backend/opengl/command/cmd_clear.h"
#include "render/irender.h"

namespace zidian{
    void CmdClear::execute(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}