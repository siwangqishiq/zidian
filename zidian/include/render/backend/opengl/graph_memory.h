#pragma once

#include "render/backend/opengl/glheader.h"
#include <vector>

namespace zidian{
    class GraphMemoryAllocator{
    public:
        GraphMemoryAllocator();
        ~GraphMemoryAllocator();
    private:
        GLuint m_vao = 0;
        std::vector<GLuint> m_vbos;
    };
}