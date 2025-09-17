#pragma once

#include "render/irender.h"

namespace zidian{
    class OpenglRender : public IRender{
        virtual int initEnv();
    };
}


