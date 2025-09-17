#pragma once

#include "render/irender.h"

namespace zidian{
    class VulkanRender : public IRender{
        virtual int initEnv();
    };
}
