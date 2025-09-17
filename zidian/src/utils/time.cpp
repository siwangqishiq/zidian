#include "utils/time.h"
#include <chrono>

namespace zidian{
    long CurrentTimeSeconds(){
        using namespace std;
        using namespace std::chrono;
        auto now = system_clock::now();
        auto us = duration_cast<seconds>(now.time_since_epoch());
        return us.count();
    }

    long long CurrentTimeMicro(){
        using namespace std;
        using namespace std::chrono;
        auto now = system_clock::now();
        auto us = duration_cast<microseconds>(now.time_since_epoch());
        return us.count();
    }

    long long CurrentTimeMillis(){
        using namespace std;
        using namespace std::chrono;
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        return static_cast<long>(ms.count());
    }

    float CurrentTimeMillisFloat(){
        auto t = CurrentTimeMillis();
        return t / 1000.0f;
    }
}