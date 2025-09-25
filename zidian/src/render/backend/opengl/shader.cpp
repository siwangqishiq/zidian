#include "render/backend/opengl/shader.h"
#include "utils/log.h"
#include "resource/asset_manager.h"
#include "glm/gtc/type_ptr.hpp"

namespace zidian{
    std::unique_ptr<ShaderManager> ShaderManager::m_instance = nullptr;

    //编译glsl的 shader
    GLuint CompileShader(GLenum shaderType, const char* shaderSource){
        GLuint shader = glCreateShader(shaderType);
        if (shader == 0) {
            Log::e(TAG_SHADER , "create shader failed : %s" , shaderSource);
            glDeleteShader(shader);
            return 0;
        }
        const char* shaderCode = shaderSource;
        if (shaderCode == nullptr) {
            Log::e(TAG_SHADER , "load shader code from %s failed" , shaderSource);
            glDeleteShader(shader);
            return 0;
        }

        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);

        GLint compileResult = GL_TRUE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
        if (compileResult == GL_FALSE) {
            char szLog[1024] = { 0 };
            GLsizei logLen = 0;
            glGetShaderInfoLog(shader, 1024, &logLen, szLog);
            Log::e(TAG_SHADER , "Compile Shader fail error log: %s \nshader code:\n%s\n", szLog, shaderCode);
            glDeleteShader(shader);
            shader = 0;
        }
        return shader;
    }

    GLuint CreateGPUProgram(const char* vsShaderSource, const char* fsShaderSource){
         GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsShaderSource);
        GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsShaderSource);
        //Attach
        GLuint program = glCreateProgram();
        glAttachShader(program, vsShader);
        glAttachShader(program, fsShader);

        //Link
        glLinkProgram(program);
        GLint linkResult = GL_TRUE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
        if(linkResult == GL_FALSE){
            char szLog[1024] = { 0 };
            GLsizei logLen = 0;
            glGetProgramInfoLog(program, 1024, &logLen, szLog);
            Log::e(TAG_SHADER, "Compile program fail error log: %s \nvs shader code:\n%s\nfs shader code:\n%s\n" , szLog , vsShaderSource , fsShaderSource);
            glDeleteShader(program);
            program = 0;
            return program;
        }

        //Clear
        glDetachShader(program, vsShader);
        glDetachShader(program, fsShader);
        glDeleteShader(vsShader);
        glDeleteShader(fsShader);

        Log::w(TAG_SHADER , "program = %d" , program);
        return program;
    }

    GLuint CreateGPUProgramByAsset(std::string vsFilePath , std::string fsFilePath){
        return CreateGPUProgram(
                ShaderManager::getInstance()->readShaderSrc(vsFilePath).c_str() ,
                ShaderManager::getInstance()->readShaderSrc(fsFilePath).c_str());
    }

    void Shader::useShader(){
        glUseProgram(m_programId);
    }

    void Shader::setUniformInt(std::string key , int value){
        GLint loc = findUniformLocation(key);
        glUniform1i(loc , value);
    }

    void Shader::setUniformFloat(std::string key , float value){
        GLint loc = findUniformLocation(key);
        glUniform1f(loc , value);
    }

    void Shader::setUniformMat3(std::string key, glm::mat3 mat) {
        GLint loc = findUniformLocation(key);
        glUniformMatrix3fv(loc , 1 , GL_FALSE , glm::value_ptr(mat));
    }

    void Shader::setUniformMat4(std::string key , glm::mat4 mat){
        GLint loc = findUniformLocation(key);
        glUniformMatrix4fv(loc , 1 , GL_FALSE , glm::value_ptr(mat));
    }

    void Shader::setUniformVec4(std::string key , glm::vec4 vec){
        GLint loc = findUniformLocation(key);
        glUniform4fv(loc , 1 ,  (float *)(&vec[0]));
    }

    void Shader::setUniformVec2(std::string key , glm::vec2 vec){
        GLint loc = findUniformLocation(key);
        glUniform2fv(loc , 1 ,  (float *)(&vec[0]));
    }

    void Shader::setUniformVec3(std::string key , float x , float y , float z){
        GLint loc = findUniformLocation(key);
        glUniform3f(loc , x , y , z);
    }

    void Shader::setUniformVec3(std::string key ,glm::vec3 value){
        GLint loc = findUniformLocation(key);
        glUniform3fv(loc , 1 , (float *)(&value[0]));
    }

    void Shader::setUniformVec2(std::string key , float x , float y){
        GLint loc = findUniformLocation(key);
        glUniform2f(loc , x , y);
    }

    void Shader::setUniformIVec2(std::string key , int x , int y){
        GLint loc = findUniformLocation(key);
        glUniform2i(loc , x,  y);
    }

    void Shader::setUniformBool(std::string key , bool value){
        GLint loc = findUniformLocation(key);
        glUniform1i(loc , value);
    }

    int Shader::findUniformLocation(std::string key){
        if(unifromLocs.find(key) != unifromLocs.end()){
            return unifromLocs[key];
        }

        const int loc = glGetUniformLocation(this->m_programId , key.c_str());
        if(loc >= 0){
            unifromLocs[key] = loc;
        }
        return loc;
    }

    void Shader::deleteSelf(){
        if(m_programId != 0){
            glDeleteShader(m_programId);
        }
    }

    std::unique_ptr<ShaderManager> &ShaderManager::getInstance(){
        if(m_instance == nullptr){
            m_instance = std::make_unique<ShaderManager>();
        }
        return m_instance;
    }

    std::string ShaderManager::readShaderSrc(std::string shaderPath){
        std::string shadrHeadSrc =   
        #ifdef __ANDROID__
        "#version 310 es \n";
        #elif defined(__APPLE__) && defined(__arm64__)
        "#version 410 core \n";
        #elif __ARM_ARCH
        "#version 330 es \n";
        #else
        "#version 410 core \n";
        #endif
        
        std::string shaderBodySrc = AssetManager::getInstance()->readAssetFileAsString(shaderPath);
        return std::string(shadrHeadSrc + shaderBodySrc);
    }

    Shader ShaderManager::loadAssetShader(ShaderMeta &meta){
        return loadAssetShader(meta.name, meta.vertex_path, meta.fragment_path);
    }

    Shader ShaderManager::loadAssetShader(std::string shaderName , std::string vertexPath, 
            std::string frgPath){
        if(m_shader_map.find(shaderName) != m_shader_map.end()){ //found shader by shader name direct return
            return m_shader_map.at(shaderName);
        }
        return loadShader(shaderName , readShaderSrc(vertexPath) , readShaderSrc(frgPath));
    }

    Shader ShaderManager::loadShader(std::string shaderName , std::string vertex_src , 
            std::string frag_src){
        if(m_shader_map.find(shaderName) == m_shader_map.end()){//not found shader create a new shader
            Log::i("Shader" , "create a new shader %s " , shaderName.c_str());
            GLuint shader_id = CreateGPUProgram(vertex_src.c_str() , frag_src.c_str());
            if(shader_id > 0){
                m_shader_map[shaderName] = Shader(shader_id);
            }
        }
        return m_shader_map[shaderName];
    }

    Shader ShaderManager::getShaderByName(std::string shaderName){
        return m_shader_map[shaderName];
    }

    void ShaderManager::clear(){
        Log::i(TAG_SHADER , "shader manager clear");
        for(auto pair : m_shader_map){
            Shader shader = pair.second;
            if(glIsProgram(shader.m_programId)){
               glDeleteProgram(shader.m_programId);
            }else{
                Log::e(TAG_SHADER ,"%d is not a shader" , shader.m_programId);
            }
        }

        m_shader_map.clear();
        Log::i(TAG_SHADER ,"shader map size after clear %d" , m_shader_map.size());
    }
}


