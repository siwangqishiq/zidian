#include "resource/asset_manager.h"
#include "utils/log.h"
#include <fstream>
#include <streambuf>
#include <filesystem>
#include "config.h"

namespace zidian{
    std::unique_ptr<uint8_t[]> ReadFileAsBinary(std::string path , long &length){
        std::string filePath = path;
        Log::i("asset_manager" , "read bin file path %s" , filePath.c_str());
        try{
            std::ifstream binFile(filePath, std::ios::binary);
            
            if(!binFile.is_open()){
                Log::e("asset_manager" , "readBinaryFile %s is not existed!", filePath.c_str());
                return nullptr;
            }

            binFile.seekg(0 , std::ios::end);
            
            length = binFile.tellg();
            binFile.seekg(0 , std::ios::beg);

            Log::i("asset_manager" , "readFileAsBin size %d" , length);

            std::unique_ptr<uint8_t[]> pData = std::unique_ptr<uint8_t[]>(new uint8_t[length]);
            binFile.read((char *)pData.get() , length);
            binFile.close();
            
            return std::move(pData);
        }catch(std::exception &e){
            Log::e("asset_manager" , "readBinaryFile code %s" , e.what());
        }
        return nullptr;
    }

    std::wstring ReadFileAsWstring(const char *path){
        std::ifstream file(path);
        if(!file.is_open()){
            Log::e("asset_manager" , "readtextFile %s is not existed!", path);
            return L"";
        }

        std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
        file.close();
        return ToWideString(str);
    }


    std::unique_ptr<AssetManager> AssetManager::m_instance = nullptr;

    AssetManager::AssetManager(){
        Log::i("asset_manager" , "asset manager construct");
    }

    AssetManager::~AssetManager(){
        Log::i("asset_manager" , "~asset manager deconstruct");
    }

    std::unique_ptr<AssetManager>& AssetManager::getInstance(){
        if(m_instance == nullptr){
            #ifdef __ANDROID__
            m_instance = std::make_unique<AndroidAssetManager>();
            #else
            m_instance = std::make_unique<AssetManager>();
            #endif
        }
        return m_instance;
    }

    std::string AssetManager::assetRootDir(){
        std::filesystem::path path = std::filesystem::canonical(Global.argv[0]).parent_path();
        return (path/"../assets/").string();
    }

    std::wstring AssetManager::readAssetFileAsText(std::string path){
        std::string file_path = assetRootDir() + path;
        Log::i("asset_manager" , "read asset file path %s" , file_path.c_str());
        return ReadFileAsWstring(file_path.c_str());
    }

    std::unique_ptr<uint8_t[]> AssetManager::readAssetFileAsBinary(std::string path, long &length){
        std::string file_path = assetRootDir() + path;
        Log::i("asset_manager" , "read asset bin file path %s" , file_path.c_str());
        return ReadFileAsBinary(file_path, length);
    }


    #ifdef __ANDROID__
    std::wstring AndroidAssetManager::readAssetFileAsText(std::string path){
        std::string filePath = path;
        AAsset *file = AAssetManager_open(AndroidAssetManagerInstance , filePath.c_str(), AASSET_MODE_BUFFER);
        if(file == nullptr){
            Log::i("asset","open asset file : %s failed" , filePath.c_str());
            return L"";
        }

        Log::i("asset_manager" , "get asset file");
        size_t fileSize = AAsset_getLength(file);

        auto fileContentBuf = std::unique_ptr<char []>(new char[fileSize + 1]);
        AAsset_read(file , fileContentBuf.get() , fileSize);
        fileContentBuf[fileSize] = '\0';
        AAsset_close(file);

        std::wstring contentStr = ToWideString(std::string(fileContentBuf.get()));
        return contentStr;
    }
        
    std::unique_ptr<uint8_t[]> AndroidAssetManager::readAssetFileAsBinary(std::string path, long &length){
        Log::i("asset_manager","read android file asset : %s" , path.c_str());
        auto assetFile  = AAssetManager_open(AndroidAssetManagerInstance , path.c_str() , AASSET_MODE_STREAMING);
        length = AAsset_getLength(assetFile);
        auto fileContentBuf = std::unique_ptr<uint8_t []>(new uint8_t[length + 1]);
        AAsset_read(assetFile, contentBuf , fileSize);
        AAsset_close(assetFile);
        Log::i("asset_manager","read android file size : %d" , fileSize);
        return std::move(fileContentBuf);
    }

    #endif
}