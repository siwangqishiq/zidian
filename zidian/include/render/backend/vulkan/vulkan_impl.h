#pragma once

#include "render/irender.h"

namespace zidian{
    class VulkanRender : public IRender{
        virtual int init() override;
        virtual int initEvironment() override;
    };
}
