#include "render/backend/vulkan/vulkan_impl.h"
#include "utils/log.h"
#include "config.h"
#include "utils/log.h"

namespace zidian{
    const char *TAG = "vulkan_render";

    int VulkanRender::init(){
        Log::w("render", "init Vulkan render");
        return 0;
    }

    int VulkanRender::initEvironment(){
        Log::w("render", "init Vulkan render initEvironment");
        initVulkan();
        return 0;
    }

    void VulkanRender::initVulkan(){
#ifdef _WIN32
        _putenv("VK_LOADER_DEBUG=none");
#endif
        createInstance();
        pickPhysicalDevice();
        createSurface();
    }

    void VulkanRender::dispose(){
        // vkDestroyInstance(m_instance , nullptr);
    }

    void VulkanRender::createInstance(){
        Log::i(TAG, "init Vulkan render");
        
        vk::ApplicationInfo appInfo {
            .pApplicationName = Config.name.c_str(),
            .applicationVersion = VK_MAKE_VERSION( 1, 0, 0 ),
            .pEngineName        = "zidian",
            .engineVersion      = VK_MAKE_VERSION( 1, 0, 0 ),
            .apiVersion = vk::ApiVersion13
        };

        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo,
        };

        m_instance = vk::raii::Instance(m_context, createInfo);
    }

    void VulkanRender::pickPhysicalDevice(){
        Log::i(TAG, "pickPhysicalDevice");

    }

    void VulkanRender::createSurface(){
        
    }
}


