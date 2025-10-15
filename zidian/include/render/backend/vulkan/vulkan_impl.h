#pragma once

#include "render/irender.h"

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include "vulkan/vulkan_raii.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

extern "C" {
GLFWAPI VkResult glfwCreateWindowSurface(
            VkInstance instance,
            GLFWwindow* handle,
            const VkAllocationCallbacks* allocator,
            VkSurfaceKHR* surface
        );
}

namespace zidian{
    const bool EnableValidationLayers = true;

    //验证层
    const std::vector validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    class VulkanRender : public IRender{
    private:
        //设备级扩展
        const std::vector<const char*> deviceExtensions = {
            vk::KHRSwapchainExtensionName,
            vk::KHRSpirv14ExtensionName,
            vk::KHRSynchronization2ExtensionName,
            vk::KHRCreateRenderpass2ExtensionName
        };

        const vk::StructureChain<vk::PhysicalDeviceFeatures2, 
            vk::PhysicalDeviceVulkan13Features, 
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT> 
        featureChain = {
            {},                               
            {.dynamicRendering = true },
            {.extendedDynamicState = true }
        };

        std::vector<const char*> getRequiredExtensions();
        int findQueueFamilies(vk::raii::PhysicalDevice phyDevice);

    public:
        virtual int init() override;
        virtual int initEvironment() override;

        virtual void dispose() override;

        void initVulkan();

        void createInstance();
        void setDebugMessenger();
        void pickPhysicalDevice();
        void createLogicDevice();
        void createSurface();
        void createSwapchain();

        static vk::Bool32 debugCallback(
                vk::DebugUtilsMessageSeverityFlagBitsEXT severity, 
                vk::DebugUtilsMessageTypeFlagsEXT type, 
                const vk::DebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);
        
        vk::raii::Context m_context;
        vk::raii::Instance m_instance = nullptr;
        vk::raii::PhysicalDevice m_physical_device = nullptr;
        vk::raii::DebugUtilsMessengerEXT m_debug_messenger = nullptr;
        vk::raii::Device m_device = nullptr;

        vk::raii::Queue m_graphics_queue = nullptr;

        vk::raii::SurfaceKHR m_surface = nullptr;
        vk::raii::Queue m_present_queue = nullptr;

        int m_graphics_queue_index = -1;
        int m_present_queue_index = -1;
    };
}
