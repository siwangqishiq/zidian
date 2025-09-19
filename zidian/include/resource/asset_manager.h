#pragma once

#include <memory>
#include "utils/json.h"

namespace zidian{

    //写入字符串到文件
    inline void WriteStringToFile(const char *path , std::wstring content){
        std::ofstream file(path);
        file << ToByteString(content);
        file.close();
    }
    
    //从任意路径读取文本文件
    std::wstring ReadFileAsWstring(std::string path);

    std::string ReadFileAsString(std::string path);

    //二进制方式读取文件 
    std::unique_ptr<uint8_t[]> ReadFileAsBinary(std::string path , long &length);

    class AssetManager{
    public:
        AssetManager();

        virtual ~AssetManager();

        static std::unique_ptr<AssetManager>& getInstance();

        std::string assetRootDir();

        virtual std::wstring readAssetFileAsText(std::string path);

        virtual std::string readAssetFileAsString(std::string path);

        virtual std::unique_ptr<uint8_t[]> readAssetFileAsBinary(std::string path, long &length);
    private:
        static std::unique_ptr<AssetManager> m_instance;
    };

    #ifdef __ANDROID__
    // #include <android/bitmap.h>
    // #include <android/imagedecoder.h>
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>

    class AndroidAssetManager : public AssetManager{
    public:
        static AAssetManager *AndroidAssetManagerInstance;
        
        virtual std::wstring readAssetFileAsText(std::string path) override;
        
        virtual std::unique_ptr<uint8_t[]> readAssetFileAsBinary(
            std::string path, 
            long &length) override;

    //     virtual std::unique_ptr<uint8_t> readAssetTextureFile(std::string path ,
    //                                                      TextureFileConfig &fileConfig ,
    //                                                      bool needFlip = false) override;
    //     //二进制方式读取文件
    //     virtual unsigned char* readAssetFileAsBinRaw(std::string path ,
    //                                                     int &length) override;
    // private:
    //     std::unique_ptr<uint8_t> readAssetTextureFileByStbi(std::string path ,TextureFileConfig &fileConfig , bool needFlip);
    };
    #endif
}