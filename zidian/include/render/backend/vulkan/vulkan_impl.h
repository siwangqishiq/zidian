#pragma once

#include "render/irender.h"
#include "vulkan/vulkan.h"

namespace zidian{
    const bool EnableValidationLayers = true;

    class VulkanRender : public IRender{
    public:
        virtual int init() override;
        virtual int initEvironment() override;

        virtual void dispose() override;

        void initVulkan();

        void createInstance();
        void createSurface();
        void pickPhysicalDevice();

        VkInstance m_instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    };
}
