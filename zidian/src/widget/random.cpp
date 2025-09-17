#include "widget/random.h"

namespace zidian{
    std::default_random_engine RandomUtil::rndEngine;

    void RandomUtil::setRandomSeed(int seed){
        rndEngine.seed(seed);
        static std::uniform_int_distribution<int> u;
        u(rndEngine);
    }
    
    int RandomUtil::randomInt(int min, int max){
        static std::uniform_int_distribution<int> u(min , max);
        return u(rndEngine);
    }
    
    float RandomUtil::randomFloat(float min , float max){
        static std::uniform_real_distribution<float> u(min, max);
        return u(rndEngine);
    }
}
