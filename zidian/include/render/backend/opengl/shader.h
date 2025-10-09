#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "render/backend/opengl/glheader.h"
#include "glm/glm.hpp"
#include "render/backend/opengl/shader_constants.h"

namespace zidian{
    const std::string TAG_SHADER = "shader";

    //编译shader源码
    GLuint CompileShader(GLenum shaderType, const char* shaderSource);

    //创建完整Shader
    GLuint CreateGPUProgram(const char* vsShaderSource, const char* fsShaderSource);

    //
    GLuint CreateGPUProgramByAsset(std::string vsFilePath , std::string fsFilePath);

    class Shader{
    public:
        Shader():m_programId(0){
        }

        Shader(GLuint pid) : m_programId(pid){
        }

        //使用shader
        void useShader();

        int findUniformLocation(std::string key);

        void deleteSelf();
        
        void setUniformInt(std::string key , int value);

        void setUniformFloat(std::string key , float value);

        void setUniformMat4(std::string key , glm::mat4 mat);

        void setUniformMat3(std::string key , glm::mat3 mat);

        //设置三维向量
        void setUniformVec3(std::string key , float x , float y , float z);

        //设置二维向量
        void setUniformVec2(std::string key , float x , float y);

        void setUniformVec2(std::string key , glm::vec2 vec);

        void setUniformIVec2(std::string key , int x , int y);

        //设置三维向量
        void setUniformVec3(std::string key ,glm::vec3 value);

        //设置四维向量
        void setUniformVec4(std::string key , glm::vec4 vec);

        void setUniformBool(std::string key , bool value);

        GLuint m_programId = 0;
    protected:
        std::unordered_map<std::string , int> unifromLocs; //unifrom变量loccation
    };

    class ShaderManager{
    public:
        static std::unique_ptr<ShaderManager>& getInstance();

        std::string readShaderSrc(std::string shaderPath);

        Shader loadAssetShader(std::string shaderName , std::string vertexPath , std::string frgPath);
        Shader loadAssetShader(ShaderMeta &meta);
        
        Shader loadShader(std::string shaderName , std::string vertexSrc , std::string frgSrc);
        
        Shader getShaderByName(std::string shaderName);

        void clear();
    private:
        static std::unique_ptr<ShaderManager> m_instance;
        std::unordered_map<std::string , Shader> m_shader_map;
    };
}