#pragma once

#include <memory>
#include <mutex>
#include <functional>
#include <map>
#include <algorithm>
#include <string>
#include "input/input_common.h"

class GLFWwindow;

namespace zidian{
    struct InputEvent {
        int action = 0;
        float x = 0.0f;
        float y = 0.0f;
        int code = -1;
        void *ptr_param = nullptr;
    };


    typedef std::function<bool(InputEvent&)> OnEventCallback;
    
    class InputManager{
    public:
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;    

        static std::unique_ptr<InputManager>& getInstance();

        InputManager();
        ~InputManager();

        bool mouse_left_pressed = false;
        bool mouse_middle_pressed = false;
        bool mouse_right_pressed = false;

        void onEvent(zidian::InputEvent event);

        void addEventListener(std::string callbackId, OnEventCallback callback);

        void removeEventListener(std::string callbackId);

        int getEventListenCallbackCount();

        int getKeyState(int key_code);

        void clearCallback();

#ifndef __ANDROID__
        GLFWwindow *m_window = nullptr;

        void setWindowInstance(GLFWwindow *window){
            this->m_window = window;
        }
#endif
    private:
        static std::mutex m_mutex;
        static std::unique_ptr<InputManager> m_instance;
        std::map<std::string, OnEventCallback> m_onevent_callbacks;
    };//end class InputManager
}

