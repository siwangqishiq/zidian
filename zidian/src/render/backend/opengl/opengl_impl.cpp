#include "render/backend/opengl/opengl_impl.h"
#include "utils/log.h"
#include "render/backend/opengl/glheader.h"

#ifndef __ANDROID__
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif

namespace zidian{
    int OpenglRender::init() {
        Log::w("render", "init OpengGL render");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        return 0;
    }

    int OpenglRender::initEvironment(){
#ifndef ANDROID
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Log::e("render", "init OpengGL environment error");
            return -1;
        }
#endif
        return 0;
    }

    void OpenglRender::drawPoint(float &x, float &y , glm::vec4 &color){
        // Log::w("render", "OpenglRender::drawPoint %f, %f color:%f,%f,%f",
        //     x, y, color[0],color[1],color[2]);
    }

    void OpenglRender::setClearColor(glm::vec4 clear_color){
        glClearColor(clear_color[0], clear_color[1] ,clear_color[2] ,clear_color[3]);
    }

    void OpenglRender::clear(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}


