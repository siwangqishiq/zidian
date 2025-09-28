#pragma once
#include "render/batch/ibatch_draw.h"
#include "constants/types.h"

namespace zidian{
    class IBatchDrawPoints : public IBatchDraw{
    public:
        virtual void addPoint(float x, float y , ColorType &color, float point_size) = 0;
    };
}