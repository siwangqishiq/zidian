#include "render/backend/vulkan/vulkan_impl.h"
#include "utils/log.h"
#include "config.h"
#include "utils/log.h"

#ifndef __ANDROID__
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif

#include <vulkan/vk_platform.h>

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
        setDebugMessenger();

        pickPhysicalDevice();
        createLogicDevice();
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

        auto glfwExtensions = getRequiredExtensions();

        auto extensionProps = m_context.enumerateInstanceExtensionProperties();
        Log::i(TAG, "vulkan instance extension:");
        for(vk::ExtensionProperties &eProp : extensionProps){
            Log::i(TAG, "extensionName : %s ", eProp.extensionName);
        }//end for each

        
        auto layerProps = m_context.enumerateInstanceLayerProperties();
        Log::i(TAG, "vulkan layer props:");
        for(auto &layer : layerProps){
            Log::i(TAG, "Layer name : %s", layer.layerName);
        }//end for each

        std::vector<char const*> requiredLayers;
        if (EnableValidationLayers) {
            requiredLayers.assign(validationLayers.begin(), validationLayers.end());
        }

        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
            .ppEnabledLayerNames = requiredLayers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size()),
            .ppEnabledExtensionNames = glfwExtensions.data(),
        };

        m_instance = vk::raii::Instance(m_context, createInfo);
    }

    std::vector<const char*> VulkanRender::getRequiredExtensions(){
        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if (EnableValidationLayers) {
            extensions.push_back("VK_EXT_debug_utils");
        }
        return extensions;
    }

    void VulkanRender::setDebugMessenger(){
        if(!EnableValidationLayers){
            return;
        }

        vk::DebugUtilsMessageSeverityFlagsEXT severity_flags(
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | 
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | 
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
        );

        vk::DebugUtilsMessageTypeFlagsEXT message_type_flags(
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | 
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | 
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
        );
        
        vk::DebugUtilsMessengerCreateInfoEXT debug_create_info {
            .messageSeverity = severity_flags,
            .messageType = message_type_flags,
            .pfnUserCallback = &VulkanRender::debugCallback
        };

        m_debug_messenger = m_instance.createDebugUtilsMessengerEXT(debug_create_info);
    }

    VKAPI_ATTR vk::Bool32 VKAPI_CALL VulkanRender::debugCallback(
            vk::DebugUtilsMessageSeverityFlagBitsEXT severity, 
            vk::DebugUtilsMessageTypeFlagsEXT type, 
            const vk::DebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data){
        if(severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
            || severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
            || severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
            || severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose){
            Log::e(TAG , "validation layer: type %d : %s msg: %s", type, callback_data->pMessage);
        }
        return vk::False;
    }

    void VulkanRender::pickPhysicalDevice(){
        Log::i(TAG, "pickPhysicalDevice");

        auto physical_devices = m_instance.enumeratePhysicalDevices();
        if(physical_devices.empty()){
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        for(const auto &phy_device : physical_devices){
            vk::PhysicalDeviceProperties props = phy_device.getProperties();
            vk::PhysicalDeviceFeatures features = phy_device.getFeatures();
            Log::w(TAG,"physical device : %s geometryShader %d", 
                props.deviceName, features.geometryShader);
        }//end for each

        m_physical_device = physical_devices[0];
        Log::w(TAG,"use physical device : %s", m_physical_device.getProperties().deviceName);
    }

    void VulkanRender::createLogicDevice(){
        Log::i(TAG, "create logic device");
        int graph_queue_index = findQueueFamilies(m_physical_device);
        Log::i(TAG, "find graphics queue index = %d" , graph_queue_index);

        float queue_priority = 1.0f;
        vk::DeviceQueueCreateInfo queueCreateInfo{
            .queueFamilyIndex = static_cast<uint32_t>(graph_queue_index),
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };

        vk::DeviceCreateInfo device_create_info{
            .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueCreateInfo,
            .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
            .ppEnabledExtensionNames = deviceExtensions.data()
        };

        m_device = vk::raii::Device(m_physical_device, device_create_info);
        m_graph_queue = vk::raii::Queue(m_device, graph_queue_index, 0);
    }

    int VulkanRender::findQueueFamilies(vk::raii::PhysicalDevice phyDevice){
        std::vector<vk::QueueFamilyProperties> queueFamilyProps = phyDevice.getQueueFamilyProperties();
        
        for(int i = 0 ; i< queueFamilyProps.size() ;i++){
            vk::QueueFamilyProperties& prop = queueFamilyProps[i];
            // std::cout << "queue count : " << prop.queueCount << std::endl;
            Log::i(TAG, "queue flags: %s", vk::to_string(prop.queueFlags).c_str());
            if(prop.queueFlags | vk::QueueFlagBits::eGraphics){
                return i;
            }
        }//end for i
        return -1;
    }

    void VulkanRender::createSurface(){
        
    }
}


