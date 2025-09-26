#include "utils/color.h"

namespace zidian{
    ColorType CreateColor(uint8_t r, uint8_t g , uint8_t b , uint8_t a){
        return std::move(glm::vec4(
            static_cast<float>(r/255.0f),
            static_cast<float>(g/255.0f),
            static_cast<float>(b/255.0f),
            static_cast<float>(a/255.0f)
        ));
    }

    ColorType Colors::RED = CreateColor(255,0,0,255);
    ColorType Colors::GREEN = CreateColor(0,255,0,255);
    ColorType Colors::BLUE = CreateColor(0,0,255,255);
}