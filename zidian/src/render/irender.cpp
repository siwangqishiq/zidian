#include "render/irender.h"
#include "utils/log.h"

namespace zidian{
    void IRender::onSizeChanged(int view_width, int view_height){
    }

    void IRender::drawPoint(float &x, float &y , glm::vec4 &color, Paint &paint){
    }

    void IRender::setClearColor(glm::vec4 clear_color){
    }

    void IRender::clear(){
    }

    void IRender::dispose(){
    }

    std::shared_ptr<IBatchDrawPoints> IRender::batchDrawPoints(){
        return nullptr;
    }

    std::shared_ptr<Cmd> IRender::createCommandInstance(int cmd_type){
        return std::make_shared<Cmd>();
    }

    void IRender::setRenderThreadId(std::thread::id render_tid){
        m_render_tid = render_tid;
    }

    bool IRender::checkIsRenderThread(){
        return m_render_tid == std::this_thread::get_id();
    }
}