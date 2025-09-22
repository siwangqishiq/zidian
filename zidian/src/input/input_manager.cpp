#include "input/input_manager.h"
#include "utils/log.h"

#ifndef __ANDROID__
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#endif

namespace zidian{
    std::unique_ptr<InputManager> InputManager::m_instance = nullptr;
    std::mutex InputManager::m_mutex;

    InputManager::InputManager(){
        Log::i("input_manager","InputManager construct");
    }

    InputManager::~InputManager(){
        clearCallback();
        Log::i("input_manager","InputManager deconstructed");
    }

    std::unique_ptr<InputManager>& InputManager::getInstance(){
        if(m_instance == nullptr){
            std::lock_guard<std::mutex> lock(m_mutex);
            m_instance = std::make_unique<InputManager>();
        }
        return m_instance;
    }

    void InputManager::onEvent(InputEvent event){
        for(auto &pair : m_onevent_callbacks){
            if(pair.second != nullptr){
                bool cost = pair.second(event);
                if(cost){
                    break;
                }
            }
        }//end for each
    }

    int InputManager::getEventListenCallbackCount(){
        return m_onevent_callbacks.size();
    }

    void InputManager::clearCallback(){
        m_onevent_callbacks.clear();
    }

    void InputManager::addEventListener(std::string callbackId, OnEventCallback callback){
        m_onevent_callbacks[callbackId] = callback;
    }

    void InputManager::removeEventListener(std::string callbackId){
        m_onevent_callbacks.erase(callbackId);
    }

    int InputManager::getKeyState(int key_code){
#ifndef __ANDROID__
        if(m_window == nullptr){
            return KEY_RELEASE;
        }

        return glfwGetKey(m_window, key_code);
#else
        return KEY_RELEASE;
#endif
    }
}