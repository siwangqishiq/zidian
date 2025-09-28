#pragma once

#include "render/batch/batch_base.h"

namespace zidian{
    class BatchDrawPoints : public Batch{
    public:
        BatchDrawPoints(IRender *render):Batch(render){
        }

        virtual ~BatchDrawPoints();
    };
}