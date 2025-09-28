#include "glm/glm.hpp"
#include "constants/types.h"



namespace zidian{
    ColorType CreateColor(uint8_t r, uint8_t g , uint8_t b , uint8_t a);

    /**
    * https://tool.oschina.net/commons?type=3
    */
    class Colors{
    public:
        static ColorType RED;
        static ColorType GREEN;
        static ColorType BLUE;

        static ColorType CORAL;
        static ColorType PURPLE;
        static ColorType SKY_BLUE;
    };
}