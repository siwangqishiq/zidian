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
        createInstance();
        pickPhysicalDevice();
        createSurface();
    }

    void VulkanRender::dispose(){
        vkDestroyInstance(m_instance , nullptr);
    }

    void VulkanRender::createInstance(){
        Log::i(TAG, "init Vulkan render");
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        appInfo.pApplicationName = Config.name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "zidian";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // auto extensions = getRequiredExtensions();
        // createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        // createInfo.ppEnabledExtensionNames = extensions.data();

        // VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        // if (EnableValidationLayers) {
        //     createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        //     createInfo.ppEnabledLayerNames = validationLayers.data();

        //     populateDebugMessengerCreateInfo(debugCreateInfo);
        //     createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        // } else {
        //     createInfo.enabledLayerCount = 0;
        //     createInfo.pNext = nullptr;
        // }

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void VulkanRender::pickPhysicalDevice(){
        Log::i(TAG, "pickPhysicalDevice");

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            // if (isDeviceSuitable(device)) {
            //     physicalDevice = device;
            //     break;
            // }
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(device, &props);
            
            Log::i(TAG, "physical device name: %s type:%d", 
                props.deviceName,props.deviceType);

            //priority select discrete graphics card NVDIA or AMD or Intel 
            if(props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
                m_physical_device = device;
                break;
            }
        }

        //select the first graph card
        if(m_physical_device == VK_NULL_HANDLE && !devices.empty()){
            m_physical_device = devices[0];
        }

        if (m_physical_device == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void VulkanRender::createSurface(){
        
    }
}


