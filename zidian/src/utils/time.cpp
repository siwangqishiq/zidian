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
        return ms.count();
    }

    double CurrentTimeMillisFloat(){
        long long t = CurrentTimeMillis();
        return static_cast<double>(t) / 1000.0;
    }
}