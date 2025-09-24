#include "render/backend/opengl/opengl_impl.h"
#include "utils/log.h"

namespace zidian{
    int OpenglRender::initEnv(){
        Log::w("render", "init OpengGL render environment");
        return 0;
    }

    void OpenglRender::drawPoint(float &x, float &y , glm::vec4 &color){
        Log::w("render", "OpenglRender::drawPoint %f, %f color:%f,%f,%f",
            x, y, color[0],color[1],color[2]);
    }
}


