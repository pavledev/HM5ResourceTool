#pragma once

#include <msclr\marshal_cppstd.h>
#include "ManagedObject.hpp"
#include "../../HM5ResourceToolLibrary/include/ZTexture.hpp"

using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace HM5ResourceToolCLI
{
    public ref class ZTexture : public ManagedObject<HM5ResourceTool::ZTexture>
    {
    public:
        ZTexture();
        static bool ConvertTextureMapToDDS(const array<Byte>^ data, const unsigned int dataSize, [Out] array<Byte>^% ddsData, [Out] unsigned int% ddsDataSize);
        static bool ConvertTextureMapToTGA(const array<Byte>^ data, const unsigned int dataSize, [Out] array<Byte>^% tgaData, [Out] unsigned int% tgaDataSize);
        static bool ConvertTextureMapToBMP(const array<Byte>^ data, const unsigned int dataSize, [Out] array<Byte>^% bmpData, [Out] unsigned int% bmpDataSize);
        static bool ConvertTextureMapToPNG(const array<Byte>^ data, const unsigned int dataSize, [Out] array<Byte>^% pngData, [Out] unsigned int% pngDataSize);
        //static bool ConvertTGAToTextureMap(String^ tgaPath);
        //static bool ConvertBMPToTextureMap(String^ bmpPath);
        //static bool ConvertPNGToTextureMap(String^ pngPath);
        static String^ GetErrorMessage(HRESULT hresult);
    };
}
