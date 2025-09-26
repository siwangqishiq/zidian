#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cstdarg>

namespace zidian{
    template<typename... Args>
    std::string FormatString(std::string format , Args... args){
        int lenStr = std::snprintf(nullptr, 0, format.c_str(), args...);
        if(lenStr <= 0){
            return "";
        }

        lenStr++;
        auto pstrOut =new char[lenStr];
        std::snprintf(pstrOut, lenStr, format.c_str(), args...);
        std::string str(pstrOut);
        delete[] pstrOut;
        return str;
    }

    class Log{
        public:
            const static std::string TAG;
            const static std::string TIME_FORMAT;
            
            #define COLOR_NONE         "\033[m"
            #define COLOR_RED          "\033[0;32;31m"
            #define COLOR_LIGHT_RED    "\033[1;31m"
            #define COLOR_GREEN        "\033[0;32;32m"
            #define COLOR_LIGHT_GREEN  "\033[1;32m"
            #define COLOR_BLUE         "\033[0;32;34m"
            #define COLOR_LIGHT_BLUE   "\033[1;34m"
            #define COLOR_DARY_GRAY    "\033[1;30m"
            #define COLOR_CYAN         "\033[0;36m"
            #define COLOR_LIGHT_CYAN   "\033[1;36m"
            #define COLOR_PURPLE       "\033[0;35m"
            #define COLOR_LIGHT_PURPLE "\033[1;35m"
            #define COLOR_BROWN        "\033[0;33m"
            #define COLOR_YELLOW       "\033[1;33m"
            #define COLOR_LIGHT_GRAY   "\033[0;37m"
            #define COLOR_WHITE        "\033[1;37m"
            
            static void e(const std::string &tag , std::string msg);
            static void w(const std::string &tag , std::string msg);
            static void i(const std::string &tag , std::string msg);

            static void green_log(const std::string &tag , std::string msg);
            static void blue_log(const std::string &tag , std::string msg);
            static void purple_log(const std::string &tag , std::string msg);

            static void log(std::string msg){
                i(TAG ,msg);
            }

            template<typename... Args>
            static void e(const std::string &tag,const std::string format , Args... args){
                e(tag , FormatString(format , args...));
            }

            template<typename... Args>
            static void i(const std::string &tag,const std::string format , Args... args){
                i(tag , FormatString(format , args...));
            }

            template<typename... Args>
            static void w(const std::string &tag,const std::string format , Args... args){
                w(tag , FormatString(format , args...));
            }

            template<typename... Args>
            static void log(const std::string format , Args... args){
                log(FormatString(format , args...));
            }

            template<typename... Args>
            static void green_log(const std::string &tag,const std::string format , Args... args){
                green_log(tag , FormatString(format , args...));
            }

            template<typename... Args>
            static void blue_log(const std::string &tag,const std::string format , Args... args){
                blue_log(tag , FormatString(format , args...));
            }

            template<typename... Args>
            static void purple_log(const std::string &tag,const std::string format , Args... args){
                purple_log(tag , FormatString(format , args...));
            }
            
            static std::string currentShowTime();
    };//end class Log
}



