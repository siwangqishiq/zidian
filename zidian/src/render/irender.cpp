#include "render/irender.h"

namespace zidian{
    void IRender::onSizeChanged(int view_width, int view_height){
    }

    std::shared_ptr<Cmd> createCommandInstance(int cmd_type){
        return nullptr;
    }

    void IRender::drawPoint(float &x, float &y , glm::vec4 &color, Paint &paint){
    }

    void IRender::setClearColor(glm::vec4 clear_color){
    }

    void IRender::clear(){
    }

    void IRender::dispose(){
    }

    std::shared_ptr<Cmd> IRender::createCommandInstance(int cmd_type){
        return std::make_shared<Cmd>();
    }
}