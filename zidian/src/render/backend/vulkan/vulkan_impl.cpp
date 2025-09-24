#include "render/backend/vulkan/vulkan_impl.h"
#include "utils/log.h"

namespace zidian{
    int VulkanRender::initEnv(){
        Log::w("render", "init Vulkan render environment");
        return 0;
    }

    void VulkanRender::drawPoint(float &x, float &y , glm::vec4 &color){
        Log::w("render", "VulkanRender::drawPoint %f, %f color:%f,%f,%f",
            x, y, color[0],color[1],color[2]);
    }
}


