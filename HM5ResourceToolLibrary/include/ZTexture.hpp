#pragma once

#include <string>
#include <Windows.h>

namespace IOI
{
    class ZTextureMap;
}

namespace HM5ResourceTool
{
    class ZTexture
    {
    public:
        __declspec(dllexport) static bool ConvertTextureMapToDDS(const std::string& textPath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToDDS(const void* data, const unsigned int dataSize, void** ddsData, unsigned int& ddsSize);
        __declspec(dllexport) static bool ConvertTextureMapToDDS(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToDDS(IOI::ZTextureMap& textureMap, const std::wstring& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToTGA(const std::string& textPath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToTGA(const void* data, const unsigned int dataSize, void** tgaData, unsigned int& tgaSize);
        __declspec(dllexport) static bool ConvertTextureMapToTGA(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToTGA(IOI::ZTextureMap& textureMap, const std::wstring& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToBMP(const std::string& textPath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToBMP(const void* data, const unsigned int dataSize, void** bmpData, unsigned int& bmpSize);
        __declspec(dllexport) static bool ConvertTextureMapToBMP(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToBMP(IOI::ZTextureMap& textureMap, const std::wstring& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToPNG(const std::string& textPath, const std::string& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToPNG(const void* data, const unsigned int dataSize, void** pngData, unsigned int& pngSize);
        __declspec(dllexport) static bool ConvertTextureMapToPNG(const void* data, const unsigned int dataSize, const std::string& outputFilePath);
        __declspec(dllexport) static bool ConvertTextureMapToPNG(IOI::ZTextureMap& textureMap, const std::wstring& outputFilePath);
        __declspec(dllexport) static bool ConvertDDSToTextureMap(const std::string& ddsPath, void** data, unsigned int& dataSize);
        __declspec(dllexport) static bool ConvertTGAToTextureMap(const std::string& tgaPath, void** data, unsigned int& dataSize);
        __declspec(dllexport) static bool ConvertBMPToTextureMap(const std::string& bmpPath, void** data, unsigned int& dataSize);
        __declspec(dllexport) static bool ConvertPNGToTextureMap(const std::string& pngPath, void** data, unsigned int& dataSize);
        __declspec(dllexport) static std::string GetErrorMessage(HRESULT hresult);
    };
}
