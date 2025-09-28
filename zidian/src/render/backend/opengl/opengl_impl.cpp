#include "render/backend/opengl/opengl_impl.h"
#include "utils/log.h"

#ifndef __ANDROID__
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif

#include "render/command/types.h"
#include "render/backend/opengl/shader.h"
#include "render/backend/opengl/command/cmd_clear.h"
#include "render/backend/opengl/command/cmd_draw_point.h"
#include "render/backend/opengl/command/cmd_set_clear_color.h"
#include "config.h"

namespace zidian{
    int OpenglRender::init() {
        Log::w("render", "init OpengGL render");

#ifndef ANDROID
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        
        return 0;
    }

    int OpenglRender::initEvironment(){
#ifndef ANDROID
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Log::e("render", "init OpengGL environment error");
            return -1;
        }
#endif

        initOpenglEnv();
        return 0;
    }

    void OpenglRender::initOpenglEnv(){
        glEnable(GL_PROGRAM_POINT_SIZE);
        
        onSizeChanged(Config.view_width, Config.view_height);
    }

    void OpenglRender::onSizeChanged(int view_width, int view_height){
        glm::mat3& matrix = Render2d::getInstance()->m_screen_ndc_matrix;
        matrix[0][0] = 2.0f / view_width;
        matrix[0][1] = 0.0f;
        matrix[0][2] = 0.0f;

        matrix[1][0] = 0.0f;
        matrix[1][1] = -2.0f / view_height;
        matrix[1][2] = 0.0f;

        matrix[2][0] = -1.0f;
        matrix[2][1] =  1.0f;
        matrix[2][2] =  1.0f;

        glViewport(0, 0, view_width, view_height);
    }

    std::shared_ptr<Cmd> OpenglRender::createCommandInstance(int cmd_type) {
        std::shared_ptr<Cmd> result = nullptr;
        switch(cmd_type){
            case CMD_TYPE_DRAW_POINT:
                result = std::make_shared<CmdDrawPoint>();
                break;
            case CMD_TYPE_CLEAR:
                result = std::make_shared<CmdClear>();
                break;
            case CMD_TYPE_SET_CLEAR_COLOR:
                result = std::make_shared<CmdSetClearColor>();
                break;
            case CMD_TYPE_BATCH_DRAW_POINTS_BEGIN:
                
                break;
        }//end switch
        return result;
    }

    void OpenglRender::drawPoint(float &x, float &y , glm::vec4 &color ,Paint &paint){
        auto cmd = Render2d::getInstance()->getCommandQueue()
            ->getCurrentCommandPool()->getCommandByType(CMD_TYPE_DRAW_POINT);
        auto draw_point_cmd = std::dynamic_pointer_cast<CmdDrawPoint>(cmd);
        draw_point_cmd->putParams(x, y, color ,paint);
        Render2d::getInstance()->addCmd(cmd);
    }

    void OpenglRender::dispose() {
        ShaderManager::getInstance()->clear();
    }

    void OpenglRender::setClearColor(glm::vec4 clear_color){
        auto cmd = Render2d::getInstance()->getCommandQueue()
            ->getCurrentCommandPool()->getCommandByType(CMD_TYPE_SET_CLEAR_COLOR);
        auto setColorCmd = std::dynamic_pointer_cast<CmdSetClearColor>(cmd);
        setColorCmd->putParams(clear_color);
        Render2d::getInstance()->addCmd(cmd);
    }

    void OpenglRender::clear(){
        auto cmd = Render2d::getInstance()->getCommandQueue()->getCurrentCommandPool()->getCommandByType(CMD_TYPE_CLEAR);
        Render2d::getInstance()->addCmd(cmd);
    }
}


