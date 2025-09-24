#include "render/command/cmd_clear.h"
#include "render/irender.h"

namespace zidian{
    void CmdClear::execute(){
        m_render->clear();
    }
}