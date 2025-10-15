#include "render/backend/vulkan/vulkan_impl.h"
#include "utils/log.h"
#include "config.h"
#include "utils/log.h"
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

        createSurface();
        createLogicDevice();
        createSwapchain();
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
        Log::i(TAG, "pickPhysicalDevice count : %d", physical_devices.size());

        if(physical_devices.empty()){
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        for(const auto &phy_device : physical_devices){
            vk::PhysicalDeviceProperties props = phy_device.getProperties();
            vk::PhysicalDeviceFeatures features = phy_device.getFeatures();
            Log::w(TAG,"physical device : %s apiVersion %d , %d", 
                props.deviceName, props.apiVersion , VK_API_VERSION_1_3);// vulkan_version >1.3 < 1.4
        }//end for each

        m_physical_device = physical_devices[0];
        Log::w(TAG,"use physical device : %s", m_physical_device.getProperties().deviceName);
    }

    void VulkanRender::createLogicDevice(){
        Log::i(TAG, "create logic device");
        m_graphics_queue_index = findQueueFamilies(m_physical_device);
        Log::i(TAG, "find graphics queue index = %d" , m_graphics_queue_index);
        m_present_queue_index = m_graphics_queue_index;
        Log::i(TAG, "set present queue index = %d" , m_present_queue_index);

        float queue_priority = 1.0f;
        vk::DeviceQueueCreateInfo queueCreateInfo{
            .queueFamilyIndex = static_cast<uint32_t>(m_graphics_queue_index),
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
        m_graphics_queue = vk::raii::Queue(m_device, m_graphics_queue_index, 0);


    }

    int VulkanRender::findQueueFamilies(vk::raii::PhysicalDevice phyDevice){
        std::vector<vk::QueueFamilyProperties> queueFamilyProps = phyDevice.getQueueFamilyProperties();
        
        int find_index = -1;
        for(int i = 0 ; i< queueFamilyProps.size() ;i++){
            vk::QueueFamilyProperties& prop = queueFamilyProps[i];
            const bool support_surface = m_physical_device.getSurfaceSupportKHR(i, *m_surface);
            // std::cout << "queue count : " << prop.queueCount << std::endl;
            Log::i(TAG, "queue flags: %s \t is support surface %d", 
                vk::to_string(prop.queueFlags).c_str(), support_surface);
            
            if((prop.queueFlags & vk::QueueFlagBits::eGraphics) && support_surface){
                find_index = i;
                break;
            }
        }//end for i
        return find_index;
    }

    void VulkanRender::createSurface(){
        Log::i(TAG, "create surface");
        VkSurfaceKHR _surface;
        if(Global.windows == nullptr){
            throw std::runtime_error("glfw surface is null");
        }   

        if (glfwCreateWindowSurface(*m_instance, Global.windows, nullptr, &_surface) != 0) {
            throw std::runtime_error("failed to create window surface!");
        }
        m_surface = vk::raii::SurfaceKHR(m_instance, _surface);
    }
    
    void VulkanRender::createSwapchain(){
        Log::i(TAG, "create swapchain");

        const vk::SurfaceCapabilitiesKHR capabilities = m_physical_device.getSurfaceCapabilitiesKHR(*m_surface);
        const std::vector<vk::SurfaceFormatKHR> formats = m_physical_device.getSurfaceFormatsKHR(*m_surface);
        // Log::i(TAG, "surface capabilities : %s", vk::to_string(capabilities).c_str());


        // vk::SwapchainCreateInfoKHR create_info{
        //     .flags = vk::SwapchainCreateFlagsKHR(),
        //     .surface = m_surface,
        //     .minImageCount = 2,
        //     .imageFormat = vk::Format::eB8G8R8A8Unorm,
        //     .imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear,
        //     .imageExtent = vk::Extent2D(Global.width, Global.height),
        //     .imageArrayLayers = 1,
        //     .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        //     .preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity,
        //     .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        //     .presentMode = vk::PresentModeKHR::eFifo,
        //     .clipped = vk::True,
        //     .oldSwapchain = nullptr,
        // };
    }

}
