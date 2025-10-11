#pragma once

#include "render/irender.h"

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include "vulkan/vulkan_raii.hpp"

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

        vk::raii::Context m_context;
        vk::raii::Instance m_instance = nullptr;
        vk::raii::PhysicalDevice m_physical_device = nullptr;
    private:

    };
}
